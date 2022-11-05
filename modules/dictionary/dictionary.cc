#include <assert.h>
#include <fstream>

#include <jsoncpp/json/reader.h>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>

#include "widget/sql/sql.h"
#include "widget/time/time.h"
#include "widget/url/url.h"

#include "dictionary.h"
#include "word.h"

namespace dictionary {

using widget::RecordPtr;
using widget::Time;
using widget::Url;
using widget::SqliteWraper;

void Word::Encode() {
    english_ = Url::Encode(english_);
    interpretation_ = Url::Encode(interpretation_);
    pronunciation_ = Url::Encode(pronunciation_);
    example_ = Url::Encode(example_);
}

void Word::Output() {
    LOG(ERROR) << Url::Decode(english_) << "  "
               << Url::Decode(pronunciation_) << " "
               << Url::Decode(interpretation_);
}

Dictionary::Dictionary() : db_path_("/home/ivar/why/run/build/dictionary.db"),
    table_name_("word"), db_(SqliteWraper(db_path_)) {
    snprintf(sql_str_, sizeof(sql_str_), "create table if not exists %s ("
                "english TEXT PRIMARY KEY NOT NULL,"
                "pronunciation TEXT,"
                "interpretation TEXT NOT NULL,"
                "example TEXT,"
                "import_time DATETIME NOT NULL,"
                "last_study_time DATETIME NOT NULL,"
                "error_count INTEGER NOT NULL);", table_name_.c_str());
    bool ret = db_.CreateTable(sql_str_);
    assert(ret);
}

Dictionary::~Dictionary() {}

bool Dictionary::ParseData(const std::string& data, std::vector<Word> &ws) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(data, root)) {
        LOG(ERROR) << "error data: " << data;
        return false;
    }
    try {
        auto &value = root["words"];
        for (auto &word : value) {
            Word w(word["english"].asString(), word["chinese"].asString(), word["type"].asString());
            if (word.isMember("example")) {
                w.example_ = word["example"].asString();
            }
            if (word.isMember("pronunciation")) {
                w.pronunciation_ = word["pronunciation"].asString();
            }
            w.Encode();
            ws.push_back(w);
        }
    } catch (const std::exception& e) {
        LOG(ERROR) << "Parse error: " << e.what();
        return false;
    }

    return true;
}

bool Dictionary::IsExist(const std::string& english) {
    snprintf(sql_str_, sizeof(sql_str_), "select english from '%s' where english='%s';",
                 table_name_.c_str(), english.c_str());
    return !db_.Select(sql_str_).empty();
}

bool Dictionary::Update(std::vector<Word>& ws, bool update_time) {
    for (auto &w : ws) {
        if (update_time) {
            w.last_study_time_ = Time::Now();
        }
        w.Encode();
        if (IsExist(w.english_)) {
            snprintf(sql_str_, sizeof(sql_str_), "update %s set interpretation='%s',"
                     "last_study_time=%d,pronunciation='%s' where english='%s';",
                     table_name_.c_str(), w.interpretation_.c_str(), w.last_study_time_,
                     w.pronunciation_.c_str(), w.english_.c_str());
            db_.Update(sql_str_);
        } else {
            w.time_ = Time::Now();
            snprintf(sql_str_, sizeof(sql_str_), "insert into %s (english,interpretation, example,"
            "import_time,last_study_time,error_count,pronunciation)"
            "values ('%s','%s','%s',%d,%d,%d,'%s');",
                table_name_.c_str(), w.english_.c_str(), w.interpretation_.c_str(),
                w.example_.c_str(), w.time_, w.last_study_time_, w.error_count_,
                w.pronunciation_.c_str());
            db_.Insert(sql_str_);
        }
    }
    return true;
}

std::vector<Word> Dictionary::FindBySql(const std::string& sql) {
    std::vector<Word> ws;
    std::vector<RecordPtr> records = db_.Select(sql_str_);
    for (auto &r : records) {
        Word w(r->data()[kDbIdxEnglish], r->data()[kDbIdxPronunciation],
               r->data()[kDbIdxInterpretation]);
        w.example_ = r->data()[kDbIdxExample];
        w.time_ = std::stoi(r->data()[kDbIdxImportTime]);
        w.last_study_time_ = std::stoi(r->data()[kDbIdxLastStudyTime]);
        w.error_count_ = std::stoi(r->data()[kDbIdxErrorCount]);
        ws.push_back(w);
    }
    return ws;
}

std::vector<Word> Dictionary::FindByRange(int start_date, int end_data) {
    snprintf(sql_str_, sizeof(sql_str_), "select * from %s where import_time>%d and import_time<%d;",
             table_name_.c_str(), start_date, end_data);
    return FindBySql(sql_str_);
}

bool Dictionary::ImportFromEudic(const std::string& file_path) {
    std::ifstream ifs(file_path);
    if (ifs.fail()) {
        LOG(ERROR) << "load dic error:" << file_path;
        return false;
    }
    std::string line;
    std::vector<Word> ws;

    while (std::getline(ifs, line)) {
        if (line.size() < 10 || !((line[0] >= '0' && line[0] <= '9') || line[0] == '-')) {
            continue;
        }
        if (line[0] >= '0' && line[0] <= '9') {
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of("\""));
            if (strs.size() < 5 || strs[1].empty()) {
                LOG(ERROR) << "load dic error, bad format: " << line;
                return false;
            }
            auto& interpret = strs[5];
            if (interpret[0] == '-') {
                interpret = interpret.erase(0, 1);
            }
            Word w(strs[1], strs[3], interpret);
            ws.push_back(w);
        } else if (line[0] == '-' && ws.size() > 1) {
            auto& w = ws[ws.size() - 1];
            w.interpretation_.append("\n");
            w.interpretation_.append(line.erase(0, 1));
        }
    }

    Update(ws, false);

    return true;
}

} // namespace dictionary
