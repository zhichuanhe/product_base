#ifndef _IUH_DBUS_DEF_H
#define _IUH_DBUS_DEF_H

#ifndef PATH_LEN
#define PATH_LEN (64)
#endif

extern char IUH_DBUS_BUSNAME[PATH_LEN];
extern char IUH_DBUS_OBJPATH[PATH_LEN];
extern char IUH_DBUS_INTERFACE[PATH_LEN];

enum wid_dbus_result_no_e {
	IUH_DBUS_SUCCESS,
	IUH_DBUS_ERROR, /* general use, no detail error information*/
	IUH_ID_BE_USED,
	IUH_ID_NOT_EXIST,	
	APPLY_IF_FAIL,
	INTERFACE_NOT_BE_BINDED,
	INTERFACE_NOT_EXIST,
	IUH_BE_USING,
	IUH_IF_NOT_BE_BINDED,
	SWITCH_IS_DISABLE,
	HNB_OVER_MAX_NUM,
	HNB_ID_INVALID,
	HNB_ID_BE_REGISTERED,
	HNB_ID_UESD_BY_UE,
	HNB_REGIST_INFO_INVALID,
	HNB_ID_LARGE_THAN_MAX,
	UE_OVER_MAX_NUM,
	UE_ID_INVALID,
	UE_OVER_HNB_MAX_NUM,
	UE_ID_BE_REGISTERED,
	UE_ID_LARGE_THAN_MAX,
	IUH_TIPC_SOCK_INIT_ERROR
};

extern char IUH_DBUS_CONF_METHOD_SET_IUH_DYNAMIC_HNB_LOGIN_INTERFACE[PATH_LEN];
extern char IUH_DBUS_SECURITY_METHOD_SET_IUH_DAEMONLOG_DEBUG[PATH_LEN];
extern char IUH_DBUS_HNB_METHOD_SHOW_HNBINFO_BY_HNBID[PATH_LEN];
extern char IUH_DBUS_HNB_METHOD_SHOW_HNB_LIST[PATH_LEN];
extern char IUH_DBUS_SECURITY_METHOD_DELETE_HNB_BY_HNBID[PATH_LEN];
extern char IUH_DBUS_HNB_METHOD_SHOW_UEINFO_BY_UEID[PATH_LEN];
extern char IUH_DBUS_HNB_METHOD_SHOW_UE_LIST[PATH_LEN];
extern char IUH_DBUS_SECURITY_METHOD_DELETE_UE_BY_UEID[PATH_LEN];
extern char IUH_DBUS_IUH_SET_ASN_DEBUG_SWITCH[PATH_LEN];
extern char IUH_DBUS_IUH_SET_RNCID[PATH_LEN];
extern char IUH_DBUS_METHOD_SHOW_RUNNING_CFG[PATH_LEN];
extern char IUH_SET_PAGING_OPTIMIZE_SWITCH[PATH_LEN];
extern char IUH_FEMTO_ACL_WHITE_LIST[PATH_LEN];
extern char IUH_DBUS_IUH_TIPC_INIT[PATH_LEN];

#endif
