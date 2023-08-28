/**
 * Copyright (c) 2021 OceanBase
 * OceanBase CE is licensed under Mulan PubL v2.
 * You can use this software according to the terms and conditions of the Mulan PubL v2.
 * You may obtain a copy of Mulan PubL v2 at:
 *          http://license.coscl.org.cn/MulanPubL-2.0
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 * See the Mulan PubL v2 for more details.
 */

#include <gtest/gtest.h>
#define USING_LOG_PREFIX SERVER
#define protected public
#define private public

#include "env/ob_simple_cluster_test_base.h"
#include "lib/mysqlclient/ob_mysql_result.h"

namespace oceanbase
{
namespace unittest
{

static const char *TEST_FILE_NAME = "test_fetch_tables_plan";
static const char *CLUSTER_DISK_SIZE = "100G";
static const char *CLUSTER_MEMORY_SIZE = "32G";

static const char *TENANT_NAME = "tt1";
static const char *TENANT_DISK_SIZE = "50G";
static const char *TENANT_MEMORY_SIZE = "16G";

class TestRunCtx
{
public:
  uint64_t tenant_id_ = 0;
  int time_sec_ = 0;
};

TestRunCtx RunCtx;

class ObTestFetchTablesPlan : public ObSimpleClusterTestBase
{
public:
  ObTestFetchTablesPlan(): ObSimpleClusterTestBase(TEST_FILE_NAME, CLUSTER_DISK_SIZE, CLUSTER_MEMORY_SIZE) {}
  int batch_create_tables(ObMySQLProxy &proxy, const int64_t num);
  int drop_database_to_recyclebin(ObMySQLProxy &proxy, const char *name);
  int flashback_database(ObMySQLProxy &proxy, const char *name);
  int build_table_history(ObMySQLProxy &proxy, const int64_t num);
};

int ObTestFetchTablesPlan::batch_create_tables(
    ObMySQLProxy &proxy,
    const int64_t num)
{
  int ret = OB_SUCCESS;
  ObSqlString sql;
  int64_t affected_rows = 0;
  for (int64_t i = 0; OB_SUCC(ret) && i < num; i++) {
    sql.reset();
    if (OB_FAIL(sql.assign_fmt("CREATE TABLE t%ld(c1 int)", i))) {
    } else if (OB_FAIL(proxy.write(sql.ptr(), affected_rows))) {
    }
  }

  return ret;
}

int ObTestFetchTablesPlan::drop_database_to_recyclebin(
    ObMySQLProxy &proxy,
    const char *name)
{
  int ret = OB_SUCCESS;
  return ret;
}

int ObTestFetchTablesPlan::flashback_database(
    ObMySQLProxy &proxy,
    const char *name)
{
  int ret = OB_SUCCESS;
  return ret;
}

int ObTestFetchTablesPlan::build_table_history(
    ObMySQLProxy &proxy,
    const int64_t num)
{
  int ret = OB_SUCCESS;

  return ret;
}

TEST_F(ObTestFetchTablesPlan, create_tables)
{
  ASSERT_EQ(OB_SUCCESS, create_tenant(TENANT_NAME, TENANT_MEMORY_SIZE, TENANT_DISK_SIZE));
  ASSERT_EQ(OB_SUCCESS, get_tenant_id(RunCtx.tenant_id_));
  ASSERT_EQ(OB_SUCCESS, get_curr_simple_server().init_sql_proxy2());
  ObMySQLProxy &sql_proxy = get_curr_simple_server().get_sql_proxy2();

  ASSERT_EQ(OB_SUCCESS, batch_create_tables(sql_proxy, 100));
}

TEST_F(ObTestFetchTablesPlan, build_table_history)
{
  ObMySQLProxy &sql_proxy = get_curr_simple_server().get_sql_proxy2();
  ASSERT_EQ(OB_SUCCESS, build_table_history(sql_proxy, 100));
}

TEST_F(ObTestFetchTablesPlan, fetch_tables_with_scan)
{
  
}

TEST_F(ObTestFetchTablesPlan, fetch_tables_with_join)
{

}

TEST_F(ObTestFetchTablesPlan, fetch_tables_with_get)
{

}

} // unittest
} // oceanbase

int main(int argc, char **argv)
{
  oceanbase::unittest::init_log_and_gtest(argc, argv);
  OB_LOGGER.set_log_level("INFO");
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
