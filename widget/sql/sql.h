#pragma once

#include <stdlib.h>
#include <sqlite3.h>
#include <string>
#include <vector>
#include <memory>

namespace widget {

class Record;
typedef std::shared_ptr<Record> RecordPtr;

class SqliteWraper {
 public:
     /**
      * @brief operate db
      *
      * @param name: path of db
      */
    explicit SqliteWraper(const std::string &name);
    virtual ~SqliteWraper();

    /**
     * @brief select data by sql
     *
     * @param sql : sql str
     *
     * @return : all of records
     */
    std::vector<RecordPtr> Select(const std::string &sql);
    bool CreateTable(const std::string &sql) { return SQL(sql); }
    bool Insert(const std::string &sql) { return SQL(sql); }
    bool Update(const std::string &sql) { return SQL(sql); }

 private:
    /**
     * @brief exec sql
     *
     * @param sql
     *
     * @return on success return ture or return false and printf error msg
     */
    bool SQL(const std::string &sql);
    sqlite3 *db_ = nullptr;
    std::string name_;
};

class Record {
 public:
    explicit Record(int col_cnt) : col_cnt_(col_cnt) {}

    std::vector<std::string>& data() {
        return data_;
    }
    int col_cnt() {
        return col_cnt_;
    }
    std::string GetDataByCol(int col) {
        if (col > col_cnt_) {
            return "";
        }
        return data_[col];
    }

 private:
    std::vector<std::string> data_;
    int col_cnt_ = 0;
};

} // namespace widget
