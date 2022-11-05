#pragma once

#include <stdlib.h>
#include <string>
#include <vector>
#include <memory>

#include "widget/sql/sql.h"

#include "word.h"

namespace dictionary {

class Dictionary {
 public:
    static Dictionary& Instance() {
        static Dictionary d;
        return d;
    }
    bool ParseData(const std::string& data, std::vector<Word> &ws);
    bool Insert(const std::string& data);
    bool IsExist(const std::string& english);
    bool Update(std::vector<Word>& ws,  bool update_time = true);
    bool ImportFromEudic(const std::string& file_path);

    std::vector<Word> FindBySql(const std::string& sql);
    std::vector<Word> FindByDate(int date);
    std::vector<Word> FindByRange(int start_date, int end_data);

 private:
    Dictionary();
    virtual ~Dictionary();

    enum {
        kDbIdxEnglish,
        kDbIdxPronunciation,
        kDbIdxInterpretation,
        kDbIdxExample,
        kDbIdxImportTime,
        kDbIdxLastStudyTime,
        kDbIdxErrorCount
    };

    char sql_str_[2048];
    std::string db_path_;
    std::string table_name_;
    widget::SqliteWraper db_;
};

} // namespace dictionary
