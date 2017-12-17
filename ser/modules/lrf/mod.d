mod.o mod.d : mod.c mod.h ../../sr_module.h ../../parser/msg_parser.h \
 ../../parser/../comp_defs.h ../../parser/../str.h \
 ../../parser/../lump_struct.h ../../parser/.././parser/hf.h \
 ../../parser/.././parser/../str.h \
 ../../parser/.././parser/../comp_defs.h ../../parser/../flags.h \
 ../../parser/../ip_addr.h ../../parser/../str.h ../../parser/../dprint.h \
 ../../parser/../md5utils.h ../../parser/../config.h \
 ../../parser/../types.h ../../parser/parse_def.h \
 ../../parser/parse_cseq.h ../../parser/parse_to.h \
 ../../parser/parse_via.h ../../parser/parse_fline.h ../../parser/hf.h \
 ../../parser/../error.h ../../version.h ../../rpc.h ../../route_struct.h \
 ../../select.h ../../str.h ../../usr_avp.h ../../parser/parse_uri.h \
 ../../parser/../parser/msg_parser.h ../../socket_info.h ../../ip_addr.h \
 ../../dprint.h ../../globals.h ../../types.h ../../poll_types.h \
 ../../timer.h ../../clist.h ../../timer_ticks.h ../../locking.h \
 ../../lock_ops.h ../../fastlock.h ../../lock_alloc.h ../../mem/mem.h \
 ../../mem/../config.h ../../mem/../dprint.h ../../mem/q_malloc.h \
 ../../mem/meminfo.h ../../mem/shm_mem.h ../../mem/../lock_ops.h \
 ../../modules/tm/tm_load.h ../../modules/tm/defs.h \
 ../../modules/tm/../../sr_module.h ../../modules/tm/t_hooks.h \
 ../../modules/tm/uac.h ../../modules/tm/../../str.h \
 ../../modules/tm/dlg.h ../../modules/tm/../../parser/parse_rr.h \
 ../../modules/tm/../../parser/msg_parser.h \
 ../../modules/tm/../../parser/parse_nameaddr.h \
 ../../modules/tm/../../parser/../str.h \
 ../../modules/tm/../../parser/parse_param.h \
 ../../modules/tm/../../parser/hf.h \
 ../../modules/tm/../../parser/msg_parser.h ../../modules/tm/h_table.h \
 ../../modules/tm/../../types.h ../../modules/tm/../../md5utils.h \
 ../../modules/tm/../../usr_avp.h ../../modules/tm/../../timer.h \
 ../../modules/tm/config.h ../../modules/tm/../../hash_func.h \
 ../../modules/tm/../../str.h ../../modules/tm/../../hashes.h \
 ../../modules/tm/../../mem/shm_mem.h ../../modules/tm/lock.h \
 ../../modules/tm/../../dprint.h ../../modules/tm/../../locking.h \
 ../../modules/tm/sip_msg.h ../../modules/tm/t_reply.h \
 ../../modules/tm/../../rpc.h ../../modules/tm/../../tags.h \
 ../../modules/tm/../../parser/msg_parser.h \
 ../../modules/tm/../../globals.h ../../modules/tm/../../crc.h \
 ../../modules/tm/../../socket_info.h ../../modules/tm/../../dns_cache.h \
 ../../modules/tm/../../timer.h ../../modules/tm/../../ip_addr.h \
 ../../modules/tm/../../atomic_ops.h \
 ../../modules/tm/../../atomic/atomic_x86.h \
 ../../modules/tm/../../resolve.h ../../modules/tm/../../dns_wrappers.h \
 ../../modules/tm/t_fwd.h ../../modules/tm/../../proxy.h \
 ../../modules/tm/../../config.h ../../modules/tm/t_lookup.h \
 ../../modules/tm/t_funcs.h ../../modules/tm/../../globals.h \
 ../../modules/tm/../../udp_server.h \
 ../../modules/tm/../../msg_translator.h ../../modules/tm/../../forward.h \
 ../../modules/tm/../../route.h ../../modules/tm/../../error.h \
 ../../modules/tm/../../route_struct.h ../../modules/tm/../../str_hash.h \
 ../../modules/tm/../../mem/mem.h ../../modules/tm/../../clist.h \
 ../../modules/tm/../../proxy.h ../../modules/tm/../../stats.h \
 ../../modules/tm/../../udp_server.h ../../modules/tm/../../tcp_server.h \
 ../../modules/tm/../../mem/mem.h ../../modules/tm/../../ip_addr.h \
 ../../modules/tm/../../parser/parse_uri.h ../../modules/tm/timer.h \
 ../../modules/tm/ut.h ../../modules/tm/../../ut.h \
 ../../modules/tm/../../comp_defs.h ../../modules/tm/../../dprint.h \
 ../../modules/tm/../../error.h ../../modules/tm/../../resolve.h \
 ../../modules/tm/t_cancel.h ../dialog/dlg_mod.h ../dialog/../tm/dlg.h \
 ../dialog/../../db/db.h ../dialog/../../db/db_key.h \
 ../dialog/../../db/db_op.h ../dialog/../../db/db_val.h \
 ../dialog/../../db/../str.h ../dialog/../../db/db_con.h \
 ../dialog/../../db/db_row.h ../dialog/../../db/db_res.h \
 ../dialog/../../db/db_cap.h ../dialog/../tm/t_hooks.h \
 ../../lib/cds/serialize.h ../../lib/cds/dstring.h ../../lib/cds/sstr.h \
 ../../lib/cds/memory.h ../../mem/mem.h ../../mem/shm_mem.h ../../str.h \
 ../../lib/lost/client.h ../../lib/cds/logger.h ../../dprint.h \
 ../../lib/cds/list.h ../../lib/lost/../../version.h \
 ../../lib/lost/../../str.h ../../lib/lost/../../mem/mem.h \
 ../../lib/lost/../../mem/shm_mem.h \
 ../../lib/lost/../../parser/msg_parser.h \
 ../../lib/lost/../../parser/parse_uri.h ../../lib/lost/pidf_loc.h \
 ../../lib/lost/parsing.h /usr/include/libxml2/libxml/tree.h \
 /usr/include/libxml2/libxml/xmlversion.h \
 /usr/include/libxml2/libxml/xmlexports.h \
 /usr/include/libxml2/libxml/xmlstring.h \
 /usr/include/libxml2/libxml/xmlregexp.h \
 /usr/include/libxml2/libxml/dict.h \
 /usr/include/libxml2/libxml/xmlmemory.h \
 /usr/include/libxml2/libxml/threads.h \
 /usr/include/libxml2/libxml/globals.h \
 /usr/include/libxml2/libxml/parser.h /usr/include/libxml2/libxml/hash.h \
 /usr/include/libxml2/libxml/valid.h \
 /usr/include/libxml2/libxml/xmlerror.h \
 /usr/include/libxml2/libxml/list.h \
 /usr/include/libxml2/libxml/xmlautomata.h \
 /usr/include/libxml2/libxml/entities.h \
 /usr/include/libxml2/libxml/encoding.h \
 /usr/include/libxml2/libxml/xmlIO.h /usr/include/libxml2/libxml/SAX.h \
 /usr/include/libxml2/libxml/xlink.h /usr/include/libxml2/libxml/SAX2.h \
 /usr/include/libxml2/libxml/HTMLparser.h lost.h ../../str.h user_data.h \
 ../../ut.h ../../lib/lost/client.h ../../lib/lost/parsing.h \
 ../../lib/lost/pidf_loc.h dlg_state.h ../../modules/tm/dlg.h locsip.h \
 locsip_subscribe.h
