checker.o checker.d : checker.c ../scscf/ifc_datastruct.h ../scscf/registrar.h \
 ../scscf/../../sr_module.h ../scscf/../../parser/msg_parser.h \
 ../scscf/../../parser/../comp_defs.h ../scscf/../../parser/../str.h \
 ../scscf/../../parser/../lump_struct.h \
 ../scscf/../../parser/.././parser/hf.h \
 ../scscf/../../parser/.././parser/../str.h \
 ../scscf/../../parser/.././parser/../comp_defs.h \
 ../scscf/../../parser/../flags.h ../scscf/../../parser/../ip_addr.h \
 ../scscf/../../parser/../str.h ../scscf/../../parser/../dprint.h \
 ../scscf/../../parser/../md5utils.h ../scscf/../../parser/../config.h \
 ../scscf/../../parser/../types.h ../scscf/../../parser/parse_def.h \
 ../scscf/../../parser/parse_cseq.h ../scscf/../../parser/parse_to.h \
 ../scscf/../../parser/parse_via.h ../scscf/../../parser/parse_fline.h \
 ../scscf/../../parser/hf.h ../scscf/../../parser/../error.h \
 ../scscf/../../version.h ../scscf/../../rpc.h \
 ../scscf/../../route_struct.h ../scscf/../../select.h \
 ../scscf/../../str.h ../scscf/../../usr_avp.h ../scscf/../../locking.h \
 ../scscf/../../lock_ops.h ../scscf/../../fastlock.h \
 ../scscf/../../lock_alloc.h ../scscf/../../mem/mem.h \
 ../scscf/../../mem/../config.h ../scscf/../../mem/../dprint.h \
 ../scscf/../../mem/q_malloc.h ../scscf/../../mem/meminfo.h \
 ../scscf/../../mem/shm_mem.h ../scscf/../../mem/../lock_ops.h \
 /usr/include/libxml2/libxml/xmlschemas.h \
 /usr/include/libxml2/libxml/xmlversion.h \
 /usr/include/libxml2/libxml/xmlexports.h \
 /usr/include/libxml2/libxml/tree.h \
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
 ../../str.h checker.h ../../sr_module.h sip.h ../../data_lump_rpl.h \
 ../../parser/msg_parser.h ../../parser/contact/parse_contact.h \
 ../../parser/contact/../hf.h ../../parser/contact/../../str.h \
 ../../parser/contact/../msg_parser.h ../../parser/contact/contact.h \
 ../../parser/contact/../parse_param.h ../../parser/parse_uri.h \
 ../../parser/../str.h ../../parser/../parser/msg_parser.h \
 ../../parser/parse_from.h ../../parser/msg_parser.h \
 ../../parser/parse_content.h ../../parser/parse_disposition.h \
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
 ../../modules/tm/../../clist.h ../../modules/tm/../../dprint.h \
 ../../modules/tm/../../timer_ticks.h ../../modules/tm/config.h \
 ../../modules/tm/../../hash_func.h ../../modules/tm/../../str.h \
 ../../modules/tm/../../hashes.h ../../modules/tm/../../mem/shm_mem.h \
 ../../modules/tm/lock.h ../../modules/tm/../../dprint.h \
 ../../modules/tm/../../locking.h ../../modules/tm/sip_msg.h \
 ../../modules/tm/t_reply.h ../../modules/tm/../../rpc.h \
 ../../modules/tm/../../tags.h ../../modules/tm/../../parser/msg_parser.h \
 ../../modules/tm/../../globals.h ../../modules/tm/../../types.h \
 ../../modules/tm/../../ip_addr.h ../../modules/tm/../../poll_types.h \
 ../../modules/tm/../../crc.h ../../modules/tm/../../socket_info.h \
 ../../modules/tm/../../dns_cache.h ../../modules/tm/../../timer.h \
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
 ../../modules/tm/../../mem/mem.h ../../modules/tm/../../proxy.h \
 ../../modules/tm/../../stats.h ../../modules/tm/../../udp_server.h \
 ../../modules/tm/../../tcp_server.h ../../modules/tm/../../mem/mem.h \
 ../../modules/tm/../../ip_addr.h \
 ../../modules/tm/../../parser/parse_uri.h ../../modules/tm/timer.h \
 ../../modules/tm/ut.h ../../modules/tm/../../ut.h \
 ../../modules/tm/../../comp_defs.h ../../modules/tm/../../error.h \
 ../../modules/tm/../../resolve.h ../../modules/tm/t_cancel.h mark.h \
 ../../parser/msg_parser.h ../../lump_struct.h ../../data_lump.h \
 ../../lump_struct.h ../../parser/hf.h mod.h ../../dprint.h \
 ../../mem/mem.h ../scscf/scscf_load.h ../scscf/registrar_storage.h \
 ../scscf/../../modules/tm/tm_load.h ../scscf/../../qvalue.h \
 ../scscf/registrar_parser.h ../scscf/ifc_datastruct.h \
 ../scscf/registrar_notify.h ../scscf/ims_pm.h
