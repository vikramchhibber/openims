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
 ../../select.h ../../str.h ../../usr_avp.h auth_api.h \
 ../../parser/msg_parser.h ../../parser/hf.h ../../str.h \
 ../../socket_info.h ../../ip_addr.h ../../dprint.h ../../globals.h \
 ../../types.h ../../poll_types.h ../../timer.h ../../clist.h \
 ../../timer_ticks.h ../../locking.h ../../lock_ops.h ../../fastlock.h \
 ../../lock_alloc.h ../../mem/mem.h ../../mem/../config.h \
 ../../mem/../dprint.h ../../mem/q_malloc.h ../../mem/meminfo.h \
 ../../mem/shm_mem.h ../../mem/../lock_ops.h ../../modules/tm/tm_load.h \
 ../../modules/tm/defs.h ../../modules/tm/../../sr_module.h \
 ../../modules/tm/t_hooks.h ../../modules/tm/uac.h \
 ../../modules/tm/../../str.h ../../modules/tm/dlg.h \
 ../../modules/tm/../../parser/parse_rr.h \
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
 ../../modules/tm/../../parser/parse_uri.h \
 ../../modules/tm/../../parser/../parser/msg_parser.h \
 ../../modules/tm/timer.h ../../modules/tm/ut.h \
 ../../modules/tm/../../ut.h ../../modules/tm/../../comp_defs.h \
 ../../modules/tm/../../dprint.h ../../modules/tm/../../error.h \
 ../../modules/tm/../../resolve.h ../../modules/tm/t_cancel.h \
 ../../modules/dialog/dlg_mod.h ../../modules/dialog/../tm/dlg.h \
 ../../modules/dialog/../../db/db.h \
 ../../modules/dialog/../../db/db_key.h \
 ../../modules/dialog/../../db/db_op.h \
 ../../modules/dialog/../../db/db_val.h \
 ../../modules/dialog/../../db/../str.h \
 ../../modules/dialog/../../db/db_con.h \
 ../../modules/dialog/../../db/db_row.h \
 ../../modules/dialog/../../db/db_res.h \
 ../../modules/dialog/../../db/db_cap.h \
 ../../modules/dialog/../tm/t_hooks.h ../../lib/cds/serialize.h \
 ../../lib/cds/dstring.h ../../lib/cds/sstr.h ../../lib/cds/memory.h \
 ../../mem/mem.h ../../mem/shm_mem.h ../../str.h ../cdp/cdp_load.h \
 ../cdp/utils.h ../cdp/../../dprint.h ../cdp/../../str.h \
 ../cdp/../../mem/mem.h ../cdp/../../mem/shm_mem.h ../cdp/../../locking.h \
 ../cdp/../../pt.h ../cdp/../../globals.h ../cdp/../../timer.h \
 ../cdp/../../socket_info.h ../cdp/../../locking.h \
 /usr/include/libxml2/libxml/parser.h \
 /usr/include/libxml2/libxml/xmlversion.h \
 /usr/include/libxml2/libxml/xmlexports.h \
 /usr/include/libxml2/libxml/tree.h \
 /usr/include/libxml2/libxml/xmlstring.h \
 /usr/include/libxml2/libxml/xmlregexp.h \
 /usr/include/libxml2/libxml/dict.h /usr/include/libxml2/libxml/hash.h \
 /usr/include/libxml2/libxml/valid.h \
 /usr/include/libxml2/libxml/xmlerror.h \
 /usr/include/libxml2/libxml/list.h \
 /usr/include/libxml2/libxml/xmlautomata.h \
 /usr/include/libxml2/libxml/entities.h \
 /usr/include/libxml2/libxml/encoding.h \
 /usr/include/libxml2/libxml/xmlIO.h \
 /usr/include/libxml2/libxml/globals.h /usr/include/libxml2/libxml/SAX.h \
 /usr/include/libxml2/libxml/xlink.h /usr/include/libxml2/libxml/SAX2.h \
 /usr/include/libxml2/libxml/xmlmemory.h \
 /usr/include/libxml2/libxml/threads.h ../cdp/sem.h ../cdp/diameter.h \
 ../cdp/diameter_code_result.h ../cdp/diameter_code_avp.h \
 ../cdp/diameter_api.h ../cdp/session.h ../cdp/transaction.h \
 ../cdp/diameter_ims.h ../cdp/diameter_ims_code_app.h \
 ../cdp/diameter_ims_code_avp.h ../cdp/diameter_ims_code_cmd.h \
 ../cdp/diameter_ims_code_result.h ../cdp/diameter_epc.h \
 ../cdp/diameter_epc_code_app.h ../cdp/diameter_epc_code_avp.h \
 ../cdp/diameter_epc_code_cmd.h ../cdp/diameter_epc_code_result.h \
 ../cdp/peer.h ../cdp/config.h ../cdp_avp/mod_export.h \
 ../cdp_avp/../cdp/cdp_load.h ../cdp_avp/avp_new.h \
 ../cdp_avp/avp_new_base_data_format.h ../cdp_avp/avp_add.h \
 ../cdp_avp/avp_get.h ../cdp_avp/avp_get_base_data_format.h \
 ../cdp_avp/base.h ../cdp_avp/macros.h ../cdp_avp/nasapp.h \
 ../cdp_avp/ccapp.h ../cdp_avp/imsapp.h ../cdp_avp/epcapp.h \
 ../Client_Rf/client_rf_load.h ../Client_Rf/../../str.h registration.h \
 registrar_storage.h ../../parser/contact/contact.h \
 ../../parser/contact/../../str.h ../../parser/contact/../parse_param.h \
 registrar_subscribe.h registrar.h nat_helper.h security.h dlg_state.h \
 ../../modules/tm/dlg.h sdp_util.h ../../dset.h ../../qvalue.h \
 ../../dprint.h ../../ut.h p_persistency.h bin_pcscf.h bin.h \
 ../../mem/mem.h bin_file.h bin_db_pcscf.h release_call.h sip.h \
 ../../parser/contact/parse_contact.h ../../parser/contact/../hf.h \
 ../../parser/contact/../msg_parser.h ../../parser/contact/contact.h \
 ../../parser/digest/digest.h ../../parser/digest/digest_parser.h \
 ../../parser/digest/../../str.h ../../parser/digest/../hf.h \
 ../../parser/digest/../msg_parser.h ../../parser/parse_rr.h \
 ims_pm_pcscf.h ims_pm.h emerg.h em_numbers.h policy_control.h pcc.h
