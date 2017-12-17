sip.o sip.d : sip.c ../../str.h ../../parser/parse_expires.h \
 ../../parser/../str.h ../../parser/hf.h ../../parser/../comp_defs.h \
 ../../dprint.h ../../mem/mem.h ../../mem/../config.h \
 ../../mem/../types.h ../../mem/../dprint.h ../../mem/q_malloc.h \
 ../../mem/meminfo.h ../scscf/scscf_load.h ../scscf/../../sr_module.h \
 ../scscf/../../parser/msg_parser.h ../scscf/../../parser/../comp_defs.h \
 ../scscf/../../parser/../str.h ../scscf/../../parser/../lump_struct.h \
 ../scscf/../../parser/.././parser/hf.h ../scscf/../../parser/../flags.h \
 ../scscf/../../parser/../ip_addr.h ../scscf/../../parser/../str.h \
 ../scscf/../../parser/../dprint.h ../scscf/../../parser/../md5utils.h \
 ../scscf/../../parser/../config.h ../scscf/../../parser/parse_def.h \
 ../scscf/../../parser/parse_cseq.h ../scscf/../../parser/parse_to.h \
 ../scscf/../../parser/parse_via.h ../scscf/../../parser/parse_fline.h \
 ../scscf/../../parser/hf.h ../scscf/../../parser/../error.h \
 ../scscf/../../version.h ../scscf/../../rpc.h \
 ../scscf/../../route_struct.h ../scscf/../../select.h \
 ../scscf/../../str.h ../scscf/../../usr_avp.h \
 ../scscf/registrar_storage.h ../scscf/../../locking.h \
 ../scscf/../../lock_ops.h ../scscf/../../fastlock.h \
 ../scscf/../../lock_alloc.h ../scscf/../../mem/mem.h \
 ../scscf/../../mem/shm_mem.h ../scscf/../../mem/../dprint.h \
 ../scscf/../../mem/../lock_ops.h ../scscf/../../mem/q_malloc.h \
 ../scscf/../../modules/tm/tm_load.h ../scscf/../../modules/tm/defs.h \
 ../scscf/../../modules/tm/../../sr_module.h \
 ../scscf/../../modules/tm/t_hooks.h ../scscf/../../modules/tm/uac.h \
 ../scscf/../../modules/tm/../../str.h ../scscf/../../modules/tm/dlg.h \
 ../scscf/../../modules/tm/../../parser/parse_rr.h \
 ../scscf/../../modules/tm/../../parser/msg_parser.h \
 ../scscf/../../modules/tm/../../parser/parse_nameaddr.h \
 ../scscf/../../modules/tm/../../parser/../str.h \
 ../scscf/../../modules/tm/../../parser/parse_param.h \
 ../scscf/../../modules/tm/../../parser/hf.h \
 ../scscf/../../modules/tm/../../parser/msg_parser.h \
 ../scscf/../../modules/tm/h_table.h \
 ../scscf/../../modules/tm/../../types.h \
 ../scscf/../../modules/tm/../../md5utils.h \
 ../scscf/../../modules/tm/../../usr_avp.h \
 ../scscf/../../modules/tm/../../timer.h \
 ../scscf/../../modules/tm/../../clist.h \
 ../scscf/../../modules/tm/../../dprint.h \
 ../scscf/../../modules/tm/../../timer_ticks.h \
 ../scscf/../../modules/tm/config.h \
 ../scscf/../../modules/tm/../../hash_func.h \
 ../scscf/../../modules/tm/../../str.h \
 ../scscf/../../modules/tm/../../hashes.h \
 ../scscf/../../modules/tm/../../mem/shm_mem.h \
 ../scscf/../../modules/tm/lock.h \
 ../scscf/../../modules/tm/../../dprint.h \
 ../scscf/../../modules/tm/../../locking.h \
 ../scscf/../../modules/tm/sip_msg.h ../scscf/../../modules/tm/t_reply.h \
 ../scscf/../../modules/tm/../../rpc.h \
 ../scscf/../../modules/tm/../../tags.h \
 ../scscf/../../modules/tm/../../parser/msg_parser.h \
 ../scscf/../../modules/tm/../../globals.h \
 ../scscf/../../modules/tm/../../types.h \
 ../scscf/../../modules/tm/../../ip_addr.h \
 ../scscf/../../modules/tm/../../poll_types.h \
 ../scscf/../../modules/tm/../../crc.h \
 ../scscf/../../modules/tm/../../socket_info.h \
 ../scscf/../../modules/tm/../../dns_cache.h \
 ../scscf/../../modules/tm/../../timer.h \
 ../scscf/../../modules/tm/../../atomic_ops.h \
 ../scscf/../../modules/tm/../../atomic/atomic_x86.h \
 ../scscf/../../modules/tm/../../resolve.h \
 ../scscf/../../modules/tm/../../dns_wrappers.h \
 ../scscf/../../modules/tm/t_fwd.h \
 ../scscf/../../modules/tm/../../proxy.h \
 ../scscf/../../modules/tm/../../config.h \
 ../scscf/../../modules/tm/t_lookup.h ../scscf/../../modules/tm/t_funcs.h \
 ../scscf/../../modules/tm/../../globals.h \
 ../scscf/../../modules/tm/../../udp_server.h \
 ../scscf/../../modules/tm/../../msg_translator.h \
 ../scscf/../../modules/tm/../../forward.h \
 ../scscf/../../modules/tm/../../route.h \
 ../scscf/../../modules/tm/../../error.h \
 ../scscf/../../modules/tm/../../route_struct.h \
 ../scscf/../../modules/tm/../../str_hash.h \
 ../scscf/../../modules/tm/../../mem/mem.h \
 ../scscf/../../modules/tm/../../proxy.h \
 ../scscf/../../modules/tm/../../stats.h \
 ../scscf/../../modules/tm/../../udp_server.h \
 ../scscf/../../modules/tm/../../tcp_server.h \
 ../scscf/../../modules/tm/../../mem/mem.h \
 ../scscf/../../modules/tm/../../ip_addr.h \
 ../scscf/../../modules/tm/../../parser/parse_uri.h \
 ../scscf/../../modules/tm/../../parser/../parser/msg_parser.h \
 ../scscf/../../modules/tm/timer.h ../scscf/../../modules/tm/ut.h \
 ../scscf/../../modules/tm/../../ut.h \
 ../scscf/../../modules/tm/../../comp_defs.h \
 ../scscf/../../modules/tm/../../error.h \
 ../scscf/../../modules/tm/../../resolve.h \
 ../scscf/../../modules/tm/t_cancel.h ../scscf/../../qvalue.h \
 ../scscf/registrar_parser.h ../scscf/registrar.h \
 ../scscf/ifc_datastruct.h ../scscf/registrar_notify.h ../scscf/ims_pm.h \
 ../../modules/tm/tm_load.h sip.h ../../data_lump_rpl.h \
 ../../parser/msg_parser.h ../../parser/contact/parse_contact.h \
 ../../parser/contact/../hf.h ../../parser/contact/../../str.h \
 ../../parser/contact/../msg_parser.h ../../parser/contact/contact.h \
 ../../parser/contact/../parse_param.h ../../parser/parse_uri.h \
 ../../parser/parse_from.h ../../parser/msg_parser.h \
 ../../parser/parse_content.h ../../parser/parse_disposition.h mark.h \
 ../../parser/msg_parser.h ../../lump_struct.h ../../data_lump.h \
 ../../lump_struct.h ../../parser/hf.h checker.h \
 ../scscf/ifc_datastruct.h ../../sr_module.h mod.h
