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

#define USING_LOG_PREFIX SERVER

#include "observer/virtual_table/ob_all_virtual_os_process.h"
#include "observer/ob_server_struct.h"

namespace oceanbase
{
namespace observer
{
ObAllVirtualOSProcess::ObAllVirtualOSProcess() : ObVirtualTableScannerIterator(){

}

ObAllVirtualOSProcess::~ObAllVirtualOSProcess(){

}

int ObAllVirtualOSProcess::get_next_os_process_info(ObOsProcessInfo &process_info) 
{
  int ret = OB_SUCCESS;
  struct dirent *entry = NULL;
  int64_t pid = 0;
  while (OB_SUCC(ret) && pid == 0) {
    if (NULL == (entry = readdir(proc_dir_))) {
      ret = OB_ITER_END;
    } else if (0 < (pid = atoi(entry->d_name))){
      process_info.pid_ = pid;
    }
  }

  if (OB_SUCC(ret)) {

  }

  return ret;
}

int ObAllVirtualOSProcess::inner_open()
{
  int ret = OB_SUCCESS;
  const ObAddr &addr = GCTX.self_addr();

  if (!addr.ip_to_string(ip_buf_, sizeof(ip_buf_))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("fail to convert ip to string", KR(ret), K(addr));
  } else if (NULL == (proc_dir_ = opendir("/proc"))) {
    ret = OB_ERR_UNEXPECTED;
    LOG_WARN("fail to open dir /proc", KR(ret));
  }
  return ret;
}

int ObAllVirtualOSProcess::inner_get_next_row(ObNewRow *&row)
{
  int ret = OB_SUCCESS;
  ObOsProcessInfo info;

  if (OB_FAIL(get_next_os_process_info(info))) {
    if (OB_ITER_END != ret) {
      LOG_WARN("fail to get next os process info", KR(ret));
    }
  }

  if (OB_SUCC(ret)) {
    const int64_t col_count = output_column_ids_.count();
    for (int64_t i = 0; OB_SUCC(ret) && i < col_count; ++i) {
      uint64_t col_id = output_column_ids_.at(i);
      switch (col_id) {
        case SVR_IP:
          cur_row_.cells_[i].set_varchar(common::ObString::make_string(ip_buf_));
          cur_row_.cells_[i].set_collation_type(ObCharset::get_default_collation(ObCharset::get_default_charset()));
          break;
        case SVR_PORT:
          cur_row_.cells_[i].set_int(static_cast<int64_t>(GCTX.self_addr().get_port()));
          break;
        case USER:
          cur_row_.cells_[i].set_varchar(info.user_);
          break;
        case PID:
          cur_row_.cells_[i].set_int(info.pid_);
          break;
        case CPU:
          cur_row_.cells_[i].set_int(info.cpu_);
          break;
        case MEM:
          cur_row_.cells_[i].set_int(info.mem_);
          break;
        case VSZ:
          cur_row_.cells_[i].set_int(info.vsz_);
          break;
        case RSS:
          cur_row_.cells_[i].set_int(info.rss_);
          break;
        case CMD:
          cur_row_.cells_[i].set_varchar(info.cmd_);
          cur_row_.cells_[i].set_collation_type(ObCharset::get_default_collation(ObCharset::get_default_charset()));
          break;
        default: {
          ret = OB_ERR_UNEXPECTED;
          LOG_WARN("invalid col_id", KR(ret), K(col_id));
        }
      }
    }

    if (OB_SUCC(ret)) {
      row = &cur_row_;
    }
  }
  return ret;
}

}
}

