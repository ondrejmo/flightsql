// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#pragma once

#include <sqlite3.h>

#include <memory>
#include <string>

#include "arrow/flight/sql/column_metadata.h"
#include "arrow/type_fwd.h"

namespace arrow {
namespace flight {
namespace sql {
namespace sqlite {

/// \brief Create an object ColumnMetadata using the column type and
///        table name.
/// \param column_type  The SQLite type.
/// \param table        The table name.
/// \return             A Column Metadata object.
ColumnMetadata GetColumnMetadata(int column_type, const char* table);

class SqliteStatement {
 public:
  /// \brief Creates a SQLite3 statement.
  /// \param[in] db        SQLite3 database instance.
  /// \param[in] sql       SQL statement.
  /// \return              A SqliteStatement object.
  static arrow::Result<std::shared_ptr<SqliteStatement>> Create(sqlite3* db,
                                                                const std::string& sql);

  ~SqliteStatement();

  /// \brief Creates an Arrow Schema based on the results of this statement.
  /// \return              The resulting Schema.
  arrow::Result<std::shared_ptr<Schema>> GetSchema() const;

  /// \brief Steps on underlying sqlite3_stmt.
  /// \return          The resulting return code from SQLite.
  arrow::Result<int> Step();

  /// \brief Reset the state of the sqlite3_stmt.
  /// \return          The resulting return code from SQLite.
  arrow::Result<int> Reset();

  /// \brief Returns the underlying sqlite3_stmt.
  /// \return A sqlite statement.
  sqlite3_stmt* GetSqlite3Stmt() const;

  /// \brief Executes an UPDATE, INSERT or DELETE statement.
  /// \return              The number of rows changed by execution.
  arrow::Result<int64_t> ExecuteUpdate();

 private:
  sqlite3* db_;
  sqlite3_stmt* stmt_;

  SqliteStatement(sqlite3* db, sqlite3_stmt* stmt) : db_(db), stmt_(stmt) {}
};

}  // namespace sqlite
}  // namespace sql
}  // namespace flight
}  // namespace arrow
