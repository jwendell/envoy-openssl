#!/bin/bash

set -euo pipefail

uncomment.sh "$1" --comment -h \
  --uncomment-macro-redef 'BIO_R_[[:alnum:]_]*' \
  --uncomment-struct bio_method_st \
  --uncomment-typedef bio_info_cb \
  --uncomment-func-decl BIO_new \
  --uncomment-func-decl BIO_free \
  --uncomment-func-decl BIO_free_all \
  --uncomment-func-decl BIO_get_mem_data \
  --uncomment-func-decl BIO_up_ref \
  --uncomment-func-decl BIO_read \
  --uncomment-func-decl BIO_write \
  --uncomment-func-decl BIO_puts \
  --uncomment-func-decl BIO_pending \
  --uncomment-func-decl BIO_reset \
  --uncomment-func-decl BIO_should_read \
  --uncomment-func-decl BIO_should_write \
  --uncomment-func-decl BIO_set_retry_read \
  --uncomment-func-decl BIO_set_retry_write \
  --uncomment-func-decl BIO_clear_retry_flags \
  --uncomment-func-decl BIO_printf \
  --uncomment-func-decl BIO_read_asn1 \
  --uncomment-func-decl BIO_s_mem \
  --uncomment-func-decl BIO_new_mem_buf \
  --uncomment-func-decl BIO_mem_contents \
  --uncomment-func-decl BIO_set_mem_eof_return \
  --uncomment-func-decl BIO_new_connect \
  --uncomment-func-decl BIO_new_bio_pair \
  --uncomment-func-decl ERR_print_errors \
  --uncomment-func-decl BIO_ctrl \
  --uncomment-func-decl BIO_ctrl_get_read_request \
  --uncomment-func-decl BIO_ctrl_get_write_guarantee \
  --uncomment-func-decl BIO_shutdown_wr \
  --uncomment-regex 'BORINGSSL_MAKE_DELETER(BIO,' \
  --uncomment-regex 'BORINGSSL_MAKE_UP_REF(BIO,' \
  --uncomment-macro BIO_TYPE_MEM \
  --uncomment-macro BIO_TYPE_SOCKET \
  --uncomment-func-decl BIO_should_retry \


