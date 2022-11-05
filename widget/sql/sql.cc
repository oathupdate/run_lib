#include <sqlite3.h>
#include <glog/logging.h>

#include "sql.h"

namespace widget {

SqliteWraper::SqliteWraper(const std::string &name) : name_(name) {
    if (sqlite3_open(name.c_str(), &db_)) {
        printf("sqlite3_open %s error: %s.\n", name.c_str(), sqlite3_errmsg(db_));
    }
}

SqliteWraper::~SqliteWraper() {
    if (db_) {
        sqlite3_close(db_);
    }
}

bool SqliteWraper::SQL(const std::string &sql) {
    if (db_ == nullptr) {
      return false;
    }

    char *error = nullptr;
    if (sqlite3_exec(db_, sql.data(), NULL, NULL, &error) != SQLITE_OK) {
        LOG(ERROR) << "exec : " << sql << ". failed";
        sqlite3_free(error);
      return false;
    }

    return true;
}

std::vector<RecordPtr> SqliteWraper::Select(const std::string &sql) {
    auto cb = [] (void *data, int argc, char **argv, char **azColName) {
        auto record = std::make_shared<Record>(argc);
        for (int i = 0; i < argc; i++) {
            record->data().push_back(argv[i] ? argv[i] : "");
        }
        auto records = (std::vector<RecordPtr> *)data;
        records->push_back(record);
        return 0;
    };

    char *error = nullptr;
    std::vector<RecordPtr> data;
    int rc = sqlite3_exec(db_, sql.c_str(), cb, &data, &error);
    if (rc != SQLITE_OK) {
        printf("exec(%s) failed: %s\n", sql.c_str(), error);
        sqlite3_free(error);
    }

    return data;
}

} // namespace widget
