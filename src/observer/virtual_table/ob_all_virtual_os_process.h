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

#ifndef OB_ALL_VIRTUAL_OS_PROCESS_H_
#define OB_ALL_VIRTUAL_OS_PROCESS_H_

#include "share/ob_virtual_table_scanner_iterator.h"  // ObVirtualTableScannerIterator

namespace oceanbase
{
namespace observer
{
class ObAllVirtualOSProcess : public common::ObVirtualTableScannerIterator
{
  enum COLUMN_ID_LIST
  {
    SVR_IP = common::OB_APP_MIN_COLUMN_ID,
    SVR_PORT,
    USER,
    PID,
    CPU,
    MEM,
    VSZ,
    RSS,
    CMD,
  };
private:
  struct ObOsProcessInfo
  {
    ObString user_;
    int64_t pid_;
    int64_t cpu_;
    int64_t mem_;
    int64_t vsz_;
    int64_t rss_;
    ObString cmd_;
  };

public:
  ObAllVirtualOSProcess();
  virtual ~ObAllVirtualOSProcess();
public:
  virtual int inner_open();
  virtual int inner_get_next_row(common::ObNewRow *&row);
private:
  int get_next_os_process_info(ObOsProcessInfo &process_info);
private:
  char ip_buf_[OB_MAX_SERVER_ADDR_SIZE];
  DIR *proc_dir_;
};

}
}
#endif /* SRC_OBSERVER_VIRTUAL_TABLE_OB_ALL_VIRTUAL_SERVER_H_ */
