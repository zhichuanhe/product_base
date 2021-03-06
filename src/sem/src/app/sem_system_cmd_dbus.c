#ifdef __cplusplus
extern "C"
{
#endif
/*
  Network Platform Daemon Ethernet Port Management
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <linux/if_vlan.h>
#include <linux/sockios.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <errno.h>
#include <sys/mman.h> 
#include <linux/if.h>
#include <linux/if_arp.h>
#include <string.h>
#include <sys/ioctl.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <dbus/dbus.h>
#include <sys/syslog.h>
#include <linux/tipc.h>
#include <netdb.h>
#include <stdlib.h>

#include "sysdef/returncode.h"
#include "sysdef/sem_sysdef.h"
#include "cvm/ethernet-ioctl.h"
#include "dbus/sem/sem_dbus_def.h"
#include "sem_dbus.h"
#include "sem_common.h"
#include "sem/sem_tipc.h"
#include "product/board/board_feature.h"
#include "product/product_feature.h"
#include "board/board_define.h"
#include "sem_system_cmd_dbus.h"
#include "board/netlink.h"

#include "mtd-user.h"
#include "jffs2-user.h"

extern board_fix_param_t *local_board;
extern product_fix_param_t *product;
extern board_info_syn_t board_info_to_syn;
extern product_info_syn_t product_info_syn;

extern thread_index_sd_t thread_id_arr[MAX_SLOT_NUM];
extern sd_sockaddr_tipc_t g_send_sd_arr[MAX_SLOT_NUM];

extern DBusConnection *dcli_dbus_connection;

extern int disable_keep_alive_time;
extern int disable_keep_alive_flag_time;
extern int disable_keep_alive_start_time;

unsigned long long set_system_img_done_mask = 0x0ull;
int set_system_img_board_mask = 0;

int sor_checkfastfile(char* fastname)
{
    char result_file[64][128];		
	int ret,i,fastnum;
	FILE *fp = 	NULL;
	char cmdstr[128] = {0};
	char fastnamels[128] = {0}; 
	
	sprintf(fastnamels, "/blk/%s", fastname);    	
    sprintf(cmdstr,"sor.sh ls %s 120",fastname);	
	fp = popen( cmdstr, "r" );	
	if(fp)
	{
		i=0;
		while(i<64 && fgets( result_file[i], sizeof(result_file[i]), fp ))
			i++;
		fastnum=i;  		
		ret = pclose(fp);	
		switch (WEXITSTATUS(ret)) {
			case 0:
				for(i=0;i<fastnum;i++)
				{					if(!strncasecmp(result_file[i],fastnamels,strlen(fastnamels)))
						return 0;
				}
				return -2;
			default:
				return WEXITSTATUS(ret);
			}
	}
	else
		return -3;
}

int sor_checkimg(char* imgname)
{
	char result_file[64][128];
	char *cmdstr = "sor.sh imgls imgls 180";
	int ret,i,imgnum;
	FILE *fp = 	NULL;
	

	fp = popen( cmdstr, "r" );
	if(fp)
	{
		i=0;
		while(i<64 && fgets( result_file[i], sizeof(result_file[i]), fp ))
			i++;
		imgnum=i;

		ret = pclose(fp);
		
		switch (WEXITSTATUS(ret)) {
			case 0:
				for(i=0;i<imgnum;i++)
				{
					if(!strncasecmp(result_file[i],imgname,strlen(imgname)))
						return 0;
				}
				return -1;
			default:
				return WEXITSTATUS(ret);
			}
	}
	else
		return -2;
}

int set_boot_img_name(char* imgname)
{
	int fd; 
	int retval; 
	boot_env_t	env_args={0};	
	char *name = "bootfile";
	
	sprintf(env_args.name,name);
	sprintf(env_args.value,imgname);
	env_args.operation = SAVE_BOOT_ENV;

	fd = open("/dev/bm0",0);	
	if(fd < 0)
	{ 	
		return 1;	
	}		
	retval = ioctl(fd,BM_IOC_ENV_EXCH,&env_args);

	if(retval == -1)
	{	
	
		close(fd);
		return 2;	
	}
	close(fd);
	return 0;	
}



/*
 * set boot img on local board
 * caojia 20110829
 */
DBusMessage *sem_dbus_set_boot_img(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;
	int slot_id;
	char *version_file;
	char file_path[64] = "/blk/";
	int ret = 0;
	int timeout = 3;

	/* if product is a distributed system */
	if (!product->is_distributed)
	{
		ret = 1;
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_UINT32, &slot_id,
								DBUS_TYPE_STRING, &version_file,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		ret = 1;
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	strcat(file_path, version_file);
	sem_syslog_dbg("sem_dbus_set_boot_img slot_id : %d\n", slot_id);
	sem_syslog_dbg("sem_dbus_set_boot_img version file : %s\n", version_file);

	if (slot_id == local_board->slot_id)
	{
		ret = 1;
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	while (timeout--)
	{
		ret = sor_checkimg(version_file);
		if(ret == 0 ){
			ret = set_boot_img_name(version_file);
			if( 0 == ret)
			{
				sem_syslog_dbg("Set boot img success\n");
				ret = 0;
			}
			else
			{
				ret = 1;
				sem_syslog_dbg("Config boot_img failed\n",ret);
			}

			break;
		}
		else
		{
			ret = 2;
		}
		usleep(500000);
	}
	
	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);

	return reply;
}

int sem_set_boot_img(char *img_name)
{
	int ret = 0;

	ret = sor_checkimg(img_name);
	if(ret == 0 ){
		ret = set_boot_img_name(img_name);
		if( 0 == ret)
		{
			sem_syslog_dbg("Set boot img success\n");

			return ret;
		}
		else
		{
			ret = 0xf;
			sem_syslog_dbg("Config boot_img failed\n",ret);

			return ret;
		}

	}
	else if (ret == -1)
	{
		ret = 6;
 		return 6;
	}
	else if (ret < -1 || ret > 0xf)
	{
		ret = 0xf;
		
		return ret;
	}
	else
	{
		return ret;
	}

	return ret;
}

int set_system_img_msg_send(int slot_id, char *img_name)
{
	sem_pdu_head_t * head;
	sem_tlv_t *tlv_head;
	char *img_head = NULL;
	
	board_info_syn_t *board_info_head;
    char send_buf[SEM_TIPC_SEND_BUF_LEN];

	memset(send_buf, 0, SEM_TIPC_SEND_BUF_LEN);
	head = (sem_pdu_head_t *)send_buf;
	head->slot_id = local_board->slot_id;
	head->version = 11;
	head->length = 22;

	tlv_head = (sem_tlv_t *)(send_buf + sizeof(sem_pdu_head_t));
	tlv_head->type = SEM_SET_SYSTEM_IMG;
	tlv_head->length = 33;

	img_head = (char *)(send_buf + sizeof(sem_pdu_head_t) + sizeof(sem_tlv_t));

	strcpy(img_head, img_name);

	if (sendto(g_send_sd_arr[slot_id].sd, send_buf, SEM_TIPC_SEND_BUF_LEN, 0, \
		(struct sockaddr*)&g_send_sd_arr[slot_id].sock_addr, sizeof(struct sockaddr_tipc)) < 0)
	{
		sem_syslog_warning("send set_system_img_msg to slot %d failed\n", slot_id + 1);
		return 1;
	}
	else
	{
		sem_syslog_dbg("send set_system_img_msg to slot %d succeed\n", slot_id + 1);
	}

	return 0;
}

/*
 * show bootrom environment variable
 * niehy@autelan.com 2012-09-18
 */
int set_boot_env_var_name(char* vername,char* value)
{
	int fd; 
	int retval; 
	boot_env_t	env_args={0};	
	char *name = vername;
	char *env_value = value;
	
	sprintf(env_args.name,name);
	sprintf(env_args.value,env_value);
	env_args.operation = SAVE_BOOT_ENV;

	fd = open("/dev/bm0",0);	
	if(fd < 0)
	{ 	
		return 1;	
	}		
	retval = ioctl(fd,BM_IOC_ENV_EXCH,&env_args);

	if(retval == -1)
	{	
	
		close(fd);
		return 2;	
	}
	close(fd);
	return 0;	
}

int get_boot_env_var_name(char* vername)
{
	int fd; 
	int retval; 
	boot_env_t	env_args={0};	
	char *name = vername;
	
	sprintf(env_args.name,name);
	env_args.operation = GET_BOOT_ENV;

	fd = open("/dev/bm0",0);	
	if(fd < 0)
	{ 	
		return 1;	
	}		
	retval = ioctl(fd,BM_IOC_ENV_EXCH,&env_args);

	if(retval == -1)
	{		
	
		close(fd);
		return 2;	
	}
	else
	{		
		sprintf(vername,env_args.value); 
	}
	close(fd);
	return 0;

}

DBusMessage *sem_dbus_set_boot_env_var(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;

	char *envname;
	char *input_value;
	int ret = 0;
    char file_path[200] = {0};
	int value,board_id;
	
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_STRING, &envname,
							    DBUS_TYPE_STRING, &input_value,	
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
	}
	sem_syslog_dbg("Set environment variable : %s\n", envname);

	sem_syslog_dbg("Input value : %s  \n",input_value);
    sprintf(file_path,"/dbm/local_board/board_id");

    if(strcmp(envname,"sefile") == 0)
	{
        value = get_product_board_id(file_path, &board_id);
		if(value)
		{
			sem_syslog_dbg("Get product board name failure\n");
		}
		if(board_id == BOARD_SOFT_ID_AX7605I_2X12G12S) /*AX71_2x12g12s*/
		{
            sem_syslog_dbg("this board type  doesn't support fastfwd\n");
    	    ret = -1;
		}
		else if(board_id == BOARD_SOFT_ID_AX81_SMU)/*AX81_SMU*/
		{
            sem_syslog_dbg("this board type  doesn't support fastfwd\n");
			ret = -1;
		}
		else if(board_id == BOARD_SOFT_ID_AX81_12X)/* AX81_12x */
		{
            sem_syslog_dbg("this board type  doesn't support fastfwd\n");
			ret = -1;        
		}
		else if(board_id == BOARD_SOFT_ID_AX81_2X12G12S)/*AX81_2x12g12s*/
		{
            sem_syslog_dbg("this board type  doesn't support fastfwd\n");
			ret = -1;          
		}		
	}
	if (ret == 0)
	{
     	if(strcmp(envname,"sefile")==0)
	  {       
		
          ret=sor_checkfastfile(input_value);
		  if(0== ret)
		 {
			ret = set_boot_env_var_name(envname,input_value);
        	if(0 == ret)
        	{
        		sem_syslog_dbg("Set environment variable %s success\n",envname);
        	}
        	else if(1 == ret)
        	{
        		sem_syslog_dbg("Can't open the file\n");
        	}
        	else
        	{
        		sem_syslog_dbg("\nSet environment variable %s failure ret is %d\n",envname,ret);
        	}        			
		}		
     }	
    else 
	 {   
		ret = set_boot_env_var_name(envname, input_value);
		  if(0 == ret)
		  {
           sem_syslog_dbg("Set environment variable %s success\n",envname);     
		  }
		  else if(1 == ret)
		  {
           sem_syslog_dbg("Can't open the file\n"); 
		  }
		  else 
          {
           sem_syslog_dbg("\nSet environment variable %s failure ret is %d\n",envname,ret);
          }
		  
	 }
	}
	
	reply = dbus_message_new_method_return(msg);	
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,
        								 DBUS_TYPE_UINT32,
        								 &ret);	
	return reply;

}
int get_product_board_id(char *filename, int *board_id)
{
	int fd;
	char buff[16] = {0};
	unsigned int data;

	if((filename == NULL) || (board_id == NULL))
	{
		printf("(filename == NULL) || (board_id == NULL)\n");
		return -1;
	}

	fd = open(filename, O_RDONLY, 0);
	if (fd >= 0) 
	{
		if(read(fd, buff, 16) < 0) 
			printf("Read error : no value\n");
		close(fd);
	}
	else
	{        
		printf("Open file:%s error!\n",filename);
		return -1;
	}
	
	data = strtoul(buff, NULL, 10);
	*board_id = data;
	return 0;
}

DBusMessage *sem_dbus_set_bootcmd(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;

	char *envname;
	char *input_num_temp;
	int ret = 0;
	int board_id =0;
	int retval=0;
    int input_num;
	char file_path[200] = {0};
	
	dbus_error_init(&err);

	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_STRING, &envname,
							    DBUS_TYPE_STRING, &input_num_temp,	
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
	}
	sem_syslog_dbg("Set environment variable : %s\n", envname);
	sem_syslog_dbg("Input numeral = %s  \n",input_num_temp);    
    
	sprintf(file_path,"/dbm/local_board/board_id");
	retval = get_product_board_id(file_path,&board_id);
	if( retval ) 
		sem_syslog_dbg("Get product board_id  failure\n");

	
    input_num = strtoul(input_num_temp,NULL,10);
	sem_syslog_dbg("input_num = %d  \n",input_num);
	sem_syslog_dbg("board_id = %d  \n",board_id); 
	
	switch(input_num){
		
		case 1: 
		case 2: /*AX71_SMU */
			if ((board_id != BOARD_SOFT_ID_AX7605I_CRSMU))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}   			 
				break;
		
		case 3: /* AX71_2x12g12s*/
			if ((board_id != BOARD_SOFT_ID_AX7605I_2X12G12S))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}
			break;  				
		
		case 4:
		case 5:/*AX71_1x12g12s*/
			if ((board_id != BOARD_SOFT_ID_AX7605I_1X12G12S))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}   			 
				break;  	

		case 6: /* AX81_SMU*/
			if ((board_id != BOARD_SOFT_ID_AX81_SMU))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;

			}   			 
				break;  				
			
		case 7: 
		case 8: /*AX81_AC8C/AX81_AC12C*/
			if (!((board_id == BOARD_SOFT_ID_AX81_AC12C )||(board_id == BOARD_SOFT_ID_AX81_AC8C)))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}    				 
				break;
		case 9: 
	    case 10:/*AX81_AC4X */
			if ((board_id != BOARD_SOFT_ID_AX81_AC4X))
			{  
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}   			 
				break;    		 
		case 11: /*AX81_12x */
			if ((board_id != BOARD_SOFT_ID_AX81_12X))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}
			    break; 
		case 12: 
		case 13: /*AX81_1x12g12s*/
			if ((board_id != BOARD_SOFT_ID_AX81_1X12G12S))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}    			 
				break; 
	   case 14: /*AX81_2x12g12s*/
	   	   	if ((board_id != BOARD_SOFT_ID_AX81_2X12G12S))
			{
				sem_syslog_dbg( "input parameter error,this board type doesn't correct \n");
			    ret = -1;
			}    		
				break; 
			
		default: 
			sem_syslog_dbg("Error! Not find the item: %d\n",input_num);
			ret = -2;
				break;
	}

    if (ret == 0)
    {
	  ret = set_boot_env_var_name(envname,input_num_temp);
        
	  if(0 == ret)
	  {
		 sem_syslog_dbg("Set environment variable bootcmd success\n");
	  }
	  else if(1 == ret)
	  {
		 sem_syslog_dbg("Can't open the file\n");
	  }
	  else
	  {
		 sem_syslog_dbg("\nSet environment variable bootcmd failure ret is %d\n",ret);
	  }
    }
	
	reply = dbus_message_new_method_return(msg);	
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,
        								 DBUS_TYPE_UINT32,
        								 &ret);	
	return reply;

}

DBusMessage *sem_dbus_show_boot_env_var(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;

	char *envname;
	int ret = 0;
	
	dbus_error_init(&err);
		
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_STRING, &envname,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}

	}

	sem_syslog_dbg("show bootrom environment variable : %s\n", envname);

	ret = get_boot_env_var_name(envname);
	if(0 == ret)
	{
		sem_syslog_dbg("The environment variable value is %s\n",envname);
	}
	else if(1 == ret)
	{
		sem_syslog_dbg("Can't open the file\n");
	}
	else
	{
		sem_syslog_dbg("Bootrom environment variable %s does not exis , ret is %d \n",envname,ret);
	}

	reply = dbus_message_new_method_return(msg);	
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,
        								 DBUS_TYPE_UINT32,
        								 &ret);	
	dbus_message_iter_append_basic (&iter,
								DBUS_TYPE_STRING,
								&envname);

	return reply;
}

/*
 * set boot img on every board in the whole system
 * caojia 20111027
 */
DBusMessage *sem_dbus_set_system_img(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;

	char *version_file;
	unsigned long long ret = 0x0ull;
	int retval = 0;
	int board_on_mask = 0x0;
	int temp_mask = 0;
	int i = 0;
	int timeout = 50;

	
	dbus_error_init(&err);
		
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_STRING, &version_file,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		ret = 0xffffffffffull;
		
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT64,
									 &ret);
		return reply;
	}

	sem_syslog_dbg("set system_img version file : %s\n", version_file);

	set_system_img_done_mask = 0x0ull;
	set_system_img_board_mask = 0;
	
	board_on_mask |= (1 << local_board->slot_id);
	set_system_img_board_mask |= (1 << local_board->slot_id);

	sem_syslog_dbg("set_system_img_board_mask : %d\n", set_system_img_board_mask);

	for(i = 0; i < product->slotcount; i++) 
	{
		if ((i != local_board->slot_id) && \
			(g_send_sd_arr[i].sd > 0) && \
			(product->board_on_mask & (1 << i)))
		{
			retval = set_system_img_msg_send(i, version_file);
			if (!retval)
			{
				board_on_mask |= (1 << i);
			}
			else
			{
				ret |= (0xe << (i * 4));
			}
		}
	}

	sem_syslog_dbg("board on mask : %d\n", board_on_mask);
	sem_syslog_dbg("ret : %llx\n", ret);

	if (board_on_mask != product->board_on_mask)
	{
		temp_mask = board_on_mask ^ product->board_on_mask;
		for (i = 0; i < product->slotcount; i++)
		{
			if ((temp_mask >> i) == 0)
				break;
			
			if (((temp_mask >> i) % 2) == 1)
			{
				ret |= (0xe << (i * 4));
			}
		}
	}

	sem_syslog_dbg("ret : %llx\n", ret);

	retval = sem_set_boot_img(version_file);

	if (retval != 0xf)
	{
		retval++;
	}
	ret |= (retval << (local_board->slot_id * 4));

	sem_syslog_dbg("ret : %llx\n", ret);

	while (timeout--)
	{
		if (board_on_mask == set_system_img_board_mask)
		{
			ret |= set_system_img_done_mask;
			break;
		}
		
		sleep(1);
	}

	if (timeout < 0)
	{
		ret |= set_system_img_done_mask;
		
		temp_mask = board_on_mask ^ set_system_img_board_mask;
		for (i = 0; i < product->slotcount; i++)
		{
			if ((temp_mask >> i) == 0)
				break;
			
			if (((temp_mask >> i) % 2) == 1)
			{
				ret |= (0xe << (i * 4));
			}
		}
	}

	sem_syslog_dbg("set_system_img_done_mask : %llx\n", set_system_img_done_mask);
	sem_syslog_dbg("set_system_img_board_mask : %d\n", set_system_img_board_mask);
	sem_syslog_dbg("board_on_mask : %d\n", product->board_on_mask);
	sem_syslog_dbg("ret : %llx\n", ret);

	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
								DBUS_TYPE_UINT64,
								&ret);

	return reply;
}


int mcb_active_standby_switch_notification(int slot_id)
{
	sem_pdu_head_t * head;
	sem_tlv_t *tlv_head;
	
	board_info_syn_t *board_info_head;
    char send_buf[SEM_TIPC_SEND_BUF_LEN];

	memset(send_buf, 0, SEM_TIPC_SEND_BUF_LEN);
	head = (sem_pdu_head_t *)send_buf;
	head->slot_id = local_board->slot_id;
	head->version = 11;
	head->length = 22;

	tlv_head = (sem_tlv_t *)(send_buf + sizeof(sem_pdu_head_t));
	tlv_head->type = SEM_MCB_ACTIVE_STANDBY_SWITCH_NOTIFICATION;
	tlv_head->length = 33;
	
	if (sendto(g_send_sd_arr[slot_id].sd, send_buf, SEM_TIPC_SEND_BUF_LEN, 0, \
		(struct sockaddr*)&g_send_sd_arr[slot_id].sock_addr, sizeof(struct sockaddr_tipc)) < 0)
	{
		sem_syslog_warning("send SEM_MCB_ACTIVE_STANDBY_SWITCH_NOTIFICATION failed\n");
		return 1;
	}
	else
	{
		sem_syslog_dbg("send SEM_MCB_ACTIVE_STANDBY_SWITCH_NOTIFICATION succeed\n");
	}

	return 0;
}

/*
 * mcb active standby switch
 * caojia 20110928
 */
DBusMessage *sem_dbus_mcb_active_standby_switch(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;
	int ret = 0;
	int i = 0;
	int timeout = 10;

	int local_slot_id = local_board->slot_id;
	int remote_slot_id;
	int remote_sd = -1;
	
	if (local_board->slot_id == product->master_slot_id[0])
	{
		remote_slot_id = product->master_slot_id[1];
	}
	else
	{
		remote_slot_id = product->master_slot_id[0];
	}

	/* support distributed system, and only on active master board */
	if ((!product->is_distributed) || (!local_board->is_active_master) || \
		(!product->more_than_one_master_board_on) || \
		(product_info_syn.product_slot_board_info[local_slot_id].board_state != BOARD_READY) || \
		(product_info_syn.product_slot_board_info[remote_slot_id].board_state != BOARD_READY))
	{
		sem_syslog_dbg("### Condition not meet ###\n");
		sem_syslog_dbg("### is_active_master(%d) ###\n", local_board->is_active_master);
		sem_syslog_dbg("### more_than_one_master_board_on(%d) ###\n", product->more_than_one_master_board_on);
		sem_syslog_dbg("### [local_slot_id].board_state(%d) ###\n", product_info_syn.product_slot_board_info[local_slot_id].board_state);
		sem_syslog_dbg("### [remote_slot_id].board_state(%d) ###\n", product_info_syn.product_slot_board_info[remote_slot_id].board_state);
		
		ret = 1;
		
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	#if 1
	/* not support product ax7605i temporarily */
	if (product->product_type == XXX_YYY_AX7605I)
	{
		ret = 1;
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}
	#endif

	mcb_switch_arg_t mcb_switch_arg;
	mcb_switch_arg.switch_type = COMMAND_MODE;
	mcb_switch_arg.broadcast_domain = ALL_BROADCAST;

	for(i = 0; i < product->slotcount; i++) 
	{
		if ((i != local_board->slot_id) && \
			(g_send_sd_arr[i].sd >= 0) && \
			(product->board_on_mask & (1 << i)))
		{
			mcb_active_standby_switch_notification(i);
			sem_syslog_dbg("mcb_active_standby_switch_notification send to slot %d sd %d\n", \
				i + 1, g_send_sd_arr[i].sd);
		}
	}

	ret = product->active_stby_switch((void *)(&mcb_switch_arg));
	
	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);

	return reply;
}

DBusMessage *sem_dbus_show_system_environment(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply;
	DBusError err;
	unsigned int ret = 0, is_active_master, cpu_num, board_id;
	env_state_t envi_state;
	
	dbus_error_init(&err);
	
	memset(&envi_state, 0, sizeof(env_state_t));	
	ret = local_board->show_system_environment(&envi_state);
	if(ret != 0){
		ret = 1;
		sem_syslog_dbg("set system alarm state err! ");
	}
	
	reply = dbus_message_new_method_return(msg);

	is_active_master = local_board->is_active_master;

	cpu_num = local_board->cpu_num;
	board_id = local_board->board_id;
	sem_syslog_dbg("local_board->cpu_num = %#x\n", local_board->cpu_num);
	dbus_message_append_args(reply,
		                     DBUS_TYPE_UINT32,&is_active_master,
		                     DBUS_TYPE_UINT32,&cpu_num,
		                     DBUS_TYPE_UINT32,&board_id,
							 DBUS_TYPE_UINT32,&(envi_state.fan_state),
							 DBUS_TYPE_UINT32,&(envi_state.fan_rpm),
							 DBUS_TYPE_UINT32,&(envi_state.power_state),
							 DBUS_TYPE_INT32,&(envi_state.master_inter_temp),
							 DBUS_TYPE_INT32,&(envi_state.master_remote_temp),
							 DBUS_TYPE_INT32,&(envi_state.slave_inter_temp),
							 DBUS_TYPE_INT32,&(envi_state.slave_remote_temp),
							 DBUS_TYPE_INVALID);
	return reply;	
}

DBusMessage *sem_dbus_reset_all(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned int isFast = 0;
	int slot_id = 0;
	unsigned int reset_bitmask = 0x0;
	int ret = 0;

	sem_syslog_warning("sem hardware reset all slots\n");

	dbus_error_init(&err);

	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_UINT32, &isFast,
								DBUS_TYPE_UINT32, &slot_id,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		sem_syslog_dbg("sem_dbus_reset_all get arg failed and reset normal\n");
	}

	slot_id -= 1;

	if (slot_id == -1)
	{
		reset_bitmask = 0x3fff;
	}
	else
	{
		reset_bitmask |= (1 << slot_id);
	}

	sem_syslog_dbg("reset bitmask : 0x%x\n", reset_bitmask);

	ret = product->hardware_reset_slot(reset_bitmask, isFast, RESET_CMD_FROM_VTYSH);
	if (ret)
	{ 
		sem_syslog_dbg("hardware reset failed\n");
	}
	
	return reply;
}

int disable_keep_alive_msg_send(int slot_id, int disable_time)
{
	sem_pdu_head_t * head;
	sem_tlv_t *tlv_head;
	int *time = NULL;
	
	board_info_syn_t *board_info_head;
    char send_buf[SEM_TIPC_SEND_BUF_LEN];

	memset(send_buf, 0, SEM_TIPC_SEND_BUF_LEN);
	head = (sem_pdu_head_t *)send_buf;
	head->slot_id = local_board->slot_id;
	head->version = 11;
	head->length = 22;

	tlv_head = (sem_tlv_t *)(send_buf + sizeof(sem_pdu_head_t));
	tlv_head->type = SEM_DISABLE_KEEP_ALIVE_TEMPORARILY;
	tlv_head->length = 33;

	time = (int *)(send_buf + sizeof(sem_pdu_head_t) + sizeof(sem_tlv_t));

	*time = disable_time;

	if (sendto(g_send_sd_arr[slot_id].sd, send_buf, SEM_TIPC_SEND_BUF_LEN, 0, \
		(struct sockaddr*)&g_send_sd_arr[slot_id].sock_addr, sizeof(struct sockaddr_tipc)) < 0)
	{
		sem_syslog_warning("send disable_keep_alive_msg to slot %d failed\n", slot_id + 1);
		return 1;
	}
	else
	{
		sem_syslog_dbg("send disable_keep_alive_msg to slot %d succeed\n", slot_id + 1);
	}

	return 0;
}

DBusMessage *show_all_slot_sys_info(DBusConnection *conn, DBusMessage *msg, void *user_data)
	{
		DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
	
		int time;
		int i;
		char ret[1024]={0};
		char ret_temp[1024]={0};
		char *tem=ret;
	

	
		dbus_error_init(&err);
			
		if ((dbus_message_get_args(msg, &err,
									DBUS_TYPE_UINT32, &time,
									DBUS_TYPE_INVALID)))
		{
			
			if (dbus_error_is_set(&err)) 
			{
				sem_syslog_dbg("%s raised: %s", err.name, err.message);
				dbus_error_free(&err);
			}
			//mem info
			{
				sprintf(ret,"=========================================================================\n"
 							"\t\t\t\tSLOT %d\n" 
 							"-------------------------------------------------------------------------\n"
							"memory info:\n"
							"-----------------\n",(local_board->slot_id + 1));
				FILE* fp=NULL;
				fp = popen("free","r");
				if(NULL != fp)
				{
					if(!fread(ret_temp,1,1024,fp))
					sem_syslog_dbg("fread error\n", time);
					fclose(fp);
				}else{
					sprintf(ret_temp,"Can not get mem info\n");
				}
				strcat(ret,ret_temp);
				memset(ret_temp,0,1024);
				
			}
			//cpu info
			/*
			{
				sprintf(ret_temp,"-------------------------------------------------------------------------\n"
							"cpu info:\n"
							"-----------------\n",local_board->slot_id);
				
				strcat(ret,ret_temp);
				memset(ret_temp,0,1024);
	//			system("top -n 1 > /var/run/temp_top");
				FILE* fp=NULL;
				fp = popen("top","wr");
				if(NULL != fp)
				{
					fread(ret_temp,1,1024,fp);
					fprintf(fp,"q");
					fclose(fp);
				}else{
					sprintf(ret_temp,"Can not get cpu info\n");
				}
				strcat(ret,ret_temp);
				memset(ret_temp,0,1024);
				
				
			}
			*/
			fprintf(stderr,"%s\n",ret);

			
			reply = dbus_message_new_method_return(msg);
		
			dbus_message_iter_init_append (reply, &iter);
		
			dbus_message_iter_append_basic (&iter,
										 DBUS_TYPE_STRING,
										 &tem);
			
			return reply;
		}
	
		sem_syslog_dbg("get args error\n", time);
		return NULL;
	
	}




/*added by zhaocg for md5 subcommand 2012-10-31*/

DBusMessage *sem_dbus_md5_patch_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
	{
		DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
	
		char *patchname=NULL;
		char cmdstr[512] = {0};
		char tmp[512] = {0};
		char ret[1024]={0};
		char ret_temp[1024]={0};
		char *tem=ret;
		char *token=NULL;
	
	

	
		dbus_error_init(&err);
			
		if ((dbus_message_get_args(msg, &err,
									DBUS_TYPE_STRING, &patchname,
									DBUS_TYPE_INVALID)))
		{
			
			if (dbus_error_is_set(&err)) 
			{
				sem_syslog_dbg("%s raised: %s", err.name, err.message);
				dbus_error_free(&err);
			}
			

			sprintf(ret,"SLOT %d md5 info:",(local_board->slot_id + 1));
			
			FILE* fp=NULL;
			
			sprintf(cmdstr,"sor.sh imgmd5 patch/%s 120",patchname);
				
			fp = popen( cmdstr,"r");
				
			if(NULL != fp)
			{
				fgets(ret_temp,sizeof(ret_temp),fp);
				strcpy(tmp,ret_temp);
				token = strtok(ret_temp,"=");
				token = strtok(NULL,"\r");
				if(NULL==token)
				{
					token= strtok(tmp,":");
					token = strtok(NULL,"\r");
				}
				sprintf(ret_temp,"MD5(%s)=%s",patchname,token);
				fclose(fp);
			}
			else
			{
				sprintf(ret_temp,"Can not get mem info\n");
			}

			
			strcat(ret,ret_temp);	
	
			memset(ret_temp,0,1024);

			reply = dbus_message_new_method_return(msg);
		
			dbus_message_iter_init_append (reply, &iter);
		
			dbus_message_iter_append_basic (&iter,
										 DBUS_TYPE_STRING,
										 &tem);
			
			return reply;
		}
	
		sem_syslog_dbg("get args error\n");
		return NULL;
	
	}


DBusMessage *sem_dbus_md5_img_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
		DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
	
		char *imgname=NULL;
		char cmdstr[512] = {0};
		char ret[1024]={0};
		char ret_temp[1024]={0};
		char *tem=ret;
	

	
		dbus_error_init(&err);
			
		if ((dbus_message_get_args(msg, &err,
									DBUS_TYPE_STRING, &imgname,
									DBUS_TYPE_INVALID)))
		{
			
			if (dbus_error_is_set(&err)) 
			{
				sem_syslog_dbg("%s raised: %s", err.name, err.message);
				dbus_error_free(&err);
			}
			

			sprintf(ret,"SLOT %d md5 info:",(local_board->slot_id + 1));
			
			FILE* fp=NULL;
			
			sprintf(cmdstr,"sor.sh imgmd5 %s 300",imgname);
				
			fp = popen( cmdstr,"r");
				
			if(NULL != fp)
			{
				if(!fread(ret_temp,1,1024,fp));
				sem_syslog_dbg("fread error\n");
				fclose(fp);
			}
			else
			{
				sprintf(ret_temp,"Can not get mem info\n");
			}
				
			strcat(ret,ret_temp);
			
			memset(ret_temp,0,1024);

			reply = dbus_message_new_method_return(msg);
		
			dbus_message_iter_init_append (reply, &iter);
		
			dbus_message_iter_append_basic (&iter,
										 DBUS_TYPE_STRING,
										 &tem);
			
			return reply;
		}
	
		sem_syslog_dbg("get args error\n");
		return NULL;
}

/*ended by zhaocg for md5 subcommand*/

/*added by zhaocg for fastfwd command 2012-11-21*/

DBusMessage *sem_dbus_img_or_fastfwd_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
	{
		DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
	
		char *cmdstr;
		char file_name[64][256];
		char *tem_name[64] = {0};
		int i = 0;
		int file_num = 0;
		int slotid = 0;
	
		dbus_error_init(&err);
			
		if ((dbus_message_get_args(msg, &err,
									DBUS_TYPE_STRING, &cmdstr,
									DBUS_TYPE_INVALID)))
		{
			
			if (dbus_error_is_set(&err)) 
			{
				sem_syslog_dbg("%s raised: %s", err.name, err.message);
				dbus_error_free(&err);
			}
			
			FILE* fp=NULL;
				
			fp = popen( cmdstr,"r");
				
			if(NULL != fp)
			{
				while(i<64&&fgets(file_name[i],sizeof(file_name[i]),fp))
					i++;
				file_num = i;
				fclose(fp);
			}
			else
			{
				file_num = -1;
			}

			reply = dbus_message_new_method_return(msg);
		
			dbus_message_iter_init_append (reply, &iter);
		
			dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&file_num);
			
			for(i=0;i<file_num;i++)
			{
				tem_name[i] = file_name[i];
				dbus_message_iter_append_basic (&iter,DBUS_TYPE_STRING,&tem_name[i]);
			}
		
			return reply;
		}
	
		sem_syslog_dbg("get args error\n");
		return NULL;
	
	}

DBusMessage *sem_dbus_delete_img_or_fastfwd_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
	{
		DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
	
		char cmdstr[512] = {0};
		char *file_name = NULL;
		char buf[256] = {0};
		
		int slotid = 0;
		int ret;
		int status = 0;
	    
		dbus_error_init(&err);
			
		if ((dbus_message_get_args(msg, &err,
									DBUS_TYPE_STRING, &file_name,
									DBUS_TYPE_INVALID)))
		{
			
			if (dbus_error_is_set(&err)) 
			{
				sem_syslog_dbg("%s raised: %s", err.name, err.message);
				dbus_error_free(&err);
			}
			
			FILE* fp=NULL;
			memset(buf,0,256);	
			sprintf(cmdstr,"sor.sh ls %s 120",file_name);
	        fp = popen( cmdstr, "r" );
	        if(fp)
	        {	
		     	fgets(buf, sizeof(buf), fp ); 	
				ret= pclose(fp);	
				switch (WEXITSTATUS(ret))
				{			
				case 0: 
					if(*buf != 0)	
					{							
						sprintf(cmdstr,"sor.sh rm %s 120",file_name);
						ret=system(cmdstr);
						if(WEXITSTATUS(ret)==0)
						 status=0;/*Delete success*/
						else
						{
						  /*sem_syslog_err("return is [%d] ",WEXITSTATUS(ret));*/
						  status=-1;/*Delete failed*/
						}
					}
					else
					{
						status=1;/*file not exist*/
					}
					break;					
				default :
					sem_syslog_dbg("Delete error! Cant't get file\n");
					status=-1;
					break;									
				}	
			}	
			else	
			{
				sem_syslog_dbg("Delete error!\n"); 	
				return NULL; 
			}	

			reply = dbus_message_new_method_return(msg);
		
			dbus_message_iter_init_append (reply, &iter);
		
			dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&status);
			return reply;
		}
	
		sem_syslog_dbg("get args error\n");
		return NULL;
	
	}

/*ended by zhaocg for fastfwd command 2012-11-21*/


/*
 * Disable sem keep_alive function temporarily for seconds
 * caojia 20111102
 */
DBusMessage *sem_dbus_disable_keep_alive(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter	 iter;
	DBusError err;

	int time;
	int ret = 0;
	int retval = 0;
	int i;

	struct timeval tv;
	struct timezone tz;

	dbus_error_init(&err);
		
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_UINT32, &time,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		ret = 1;
		
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	sem_syslog_dbg("Disable Time : %d\n", time);

	for(i = 0; i < product->slotcount; i++) 
	{
		if ((i != local_board->slot_id) && \
			(g_send_sd_arr[i].sd > 0) && \
			(product->board_on_mask & (1 << i)))
		{
			retval = disable_keep_alive_msg_send(i, time);
			if (retval)
			{
				ret = 1;
			}
		}
	}

	if (disable_keep_alive_time <= 0)
	{
		gettimeofday(&tv, &tz);
		disable_keep_alive_start_time = tv.tv_sec;
		disable_keep_alive_time = time;
		disable_keep_alive_flag_time = disable_keep_alive_time;
	}
	else if (disable_keep_alive_time > 0)
	{
		if (time > disable_keep_alive_time)
		{
			gettimeofday(&tv, &tz);
			disable_keep_alive_start_time = tv.tv_sec;
			disable_keep_alive_time = time;
			disable_keep_alive_flag_time = disable_keep_alive_time;
		}
	}

	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
								DBUS_TYPE_UINT32,
								&ret);

	return reply;
}

/*
*return 0 for command success exec;
*/
int exec_system_cmd(char * cmd)
{
	int result;
	int status;
	result = system(cmd);
	sem_syslog_dbg("cmd = %s\n",cmd);
	sem_syslog_dbg("function:exec_system_cmd.result = %d\n",result);
	
    if (-1 == result) {
           return 1;
    } else if (WIFEXITED(result)) {
            status = WEXITSTATUS(result);
            if (127 == status) {
                return 1;
            } else if (126 == status) {
                return 1;
            } else {
                return 0;//success
            }
    }
	return 1;
}

int execute_system_command_msg_send(int slot_id, char *cmd)
{
	sem_pdu_head_t * head;
	sem_tlv_t *tlv_head;
	char *cmd_head = NULL;
	
	board_info_syn_t *board_info_head;
    char send_buf[SEM_TIPC_SEND_BUF_LEN];

	memset(send_buf, 0, SEM_TIPC_SEND_BUF_LEN);
	head = (sem_pdu_head_t *)send_buf;
	head->slot_id = local_board->slot_id;
	head->version = 11;
	head->length = 22;

	tlv_head = (sem_tlv_t *)(send_buf + sizeof(sem_pdu_head_t));
	tlv_head->type = SEM_EXECUTE_SYSTEM_COMMAND;
	tlv_head->length = 33;

	cmd_head = (char *)(send_buf + sizeof(sem_pdu_head_t) + sizeof(sem_tlv_t));

	strcpy(cmd_head, cmd);

	if (sendto(g_send_sd_arr[slot_id].sd, send_buf, SEM_TIPC_SEND_BUF_LEN, 0, \
		(struct sockaddr*)&g_send_sd_arr[slot_id].sock_addr, sizeof(struct sockaddr_tipc)) < 0)
	{
		sem_syslog_warning("send execute_system_command_msg to slot %d failed\n", slot_id + 1);
		return 1;
	}
	else
	{
		sem_syslog_dbg("send execute_system_command_msg to slot %d succeed\n", slot_id + 1);
	}

	return 0;
}


DBusMessage *sem_dbus_execute_system_command(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter iter;
	DBusError err;

	int ret = 0;
	int retval;
	int i;
	int slot_id;
	char *cmd;

	dbus_error_init(&err);
		
	if (!(dbus_message_get_args(msg, &err,
								DBUS_TYPE_UINT32, &slot_id,
								DBUS_TYPE_STRING, &cmd,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		ret = 2;
		
		reply = dbus_message_new_method_return(msg);
	
		dbus_message_iter_init_append (reply, &iter);
	
		dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_UINT32,
									 &ret);
		return reply;
	}

	slot_id -= 1;

	if (slot_id == local_board->slot_id)
	{
		if(exec_system_cmd(cmd))
		{
            ret = 1;
		}
		sem_syslog_dbg("execute cmd return value : %d\n", ret);
	}
	else if (slot_id == -1)
	{
		for(i = 0; i < product->slotcount; i++) 
		{
			if ((i != local_board->slot_id) && \
				(g_send_sd_arr[i].sd > 0) && \
				(product->board_on_mask & (1 << i)))
			{
				retval = execute_system_command_msg_send(i, cmd);
				if (retval)
				{
					ret = 1;
				}
			}
		}
		
		if(exec_system_cmd(cmd))
		{
            ret = 1;
		}
		sem_syslog_dbg("execute cmd return value : %d\n", ret);
	}
	else
	{
		retval = execute_system_command_msg_send(slot_id, cmd);
		if (retval)
		{
			ret = 1;
		}
	}

	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
								DBUS_TYPE_UINT32,
								&ret);

	return reply;
}

#define PATCH_EXECUTE_RESULT_BUF_LEN	1024
DBusMessage *sem_dbus_apply_patch(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned int isFast = 0;
	int slot_id = 0;
	unsigned int reset_bitmask = 0x0;
	int ret = 0;
	int status;
	char *name=NULL;
    char temp_buf[400]={0};
	int fd;
	//char execute_result_buf[PATCH_EXECUTE_RESULT_BUF_LEN] = {0};
	char *execute_result_buf = NULL;
	
	sem_syslog_dbg("Dbus to sem for apply patch!\n");

	dbus_error_init(&err);

	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_STRING,&name,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}

	sprintf(temp_buf, "/mnt/patch/%s", name);
	fd = open(temp_buf, O_RDONLY);
	if(fd < 0)
	{
		sem_syslog_dbg("apply patch %s, but the patch is not exist at /mnt/patch/\n", name);
		//ret == 2 meas the patch file is not exist
		ret = 2;
	} else {
		close(fd);
		
		sprintf(temp_buf,"cd /mnt/patch;/usr/bin/apply_patch.sh %s >& /mnt/patch.log",name);
		//sprintf(temp_buf,"vtysh -c \"apply patch %s\" >& /mnt/patch.log",name);
		sem_syslog_dbg("Execute command : %s\n",temp_buf);
		
	    status = system(temp_buf);
		
		if (-1 == status)  
	    {  
	        sem_syslog_warning("system error!");
	        ret = 1;
	    }  
	    else  
	    {  
	        sem_syslog_event("exit status value = [0x%x]\n", status);  
	  
	        if (WIFEXITED(status))  
	        {  
	            if (0 == WEXITSTATUS(status))  
	            {  
	                sem_syslog_event("run shell script %s successfully.\n",name);
					ret = 0;
	            }  
	            else  
	            {  
	                sem_syslog_warning("run shell script %s fail, script exit code: %d\n",name,WEXITSTATUS(status));  
	                ret = 1;
				}  
	        }  
	        else  
	        {  
	            sem_syslog_warning("%s exit status = [%d]\n",name,WEXITSTATUS(status));
	            ret = 1;
	        }  
	    }  
	}
	execute_result_buf = (char *)malloc(PATCH_EXECUTE_RESULT_BUF_LEN);
	if (!execute_result_buf) {
		sem_syslog_dbg("alloc buf failed\n");
		//get log failed
		ret = 3;
	} else {
		bzero(execute_result_buf, PATCH_EXECUTE_RESULT_BUF_LEN);
		fd = open("/mnt/patch.log", O_RDONLY);
		if (fd < 0) {
			sprintf(execute_result_buf, "get patch execute log failed\n");
			sem_syslog_dbg("get patch execute log failed:/mnt/patch.log\n");
		} else {
			read(fd, execute_result_buf, PATCH_EXECUTE_RESULT_BUF_LEN);
			close(fd);
		}
	}
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_INT32,
									 &ret);
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_STRING,
									 &execute_result_buf);
	free(execute_result_buf);
	
	return reply;

}


/******************************************
xufujun

********************************************/

int flash_erase_one_partition(char * mtd_device) 
{
	static const char *exe_name;
    mtd_info_t meminfo;
    int fd, clmpos = 0, clmlen = 8;
    erase_info_t erase;
    int  bbtest = 1;
    int quiet = 0;	/* true -- don't output progress */

    if ((fd = open(mtd_device, O_RDWR)) < 0) {
	return -1;
    }


    if (ioctl(fd, MEMGETINFO, &meminfo) != 0) {
	close(fd);
	return -2;
    }

    erase.length = meminfo.erasesize;
    for (erase.start = 0; erase.start < meminfo.size; erase.start += meminfo.erasesize) {
	if (bbtest) {
	    loff_t offset = erase.start;
	    int ret = ioctl(fd, MEMGETBADBLOCK, &offset);
	    if (ret > 0) {
		if (!quiet)
		continue;
	    } else if (ret < 0) {
		if (errno == EOPNOTSUPP) {
		    bbtest = 0;
		} else {
			close(fd);
		    return 1;
		}
	    }
	}

	if (ioctl(fd, MEMERASE, &erase) != 0) {
	    continue;
	}

	if (lseek (fd, erase.start, SEEK_SET) < 0) {
	    continue;
	}
    }

	close(fd);
	printf("\n");
	
    return 0;
}

/******************************************************************************/  
static int dev_fd = -1,fil_fd = -1;

/*write/rend buff*/

#define BUFSIZE (10 * 1024)

int flash_writ_boot (char *filename,char *device)
{
   int i;
   ssize_t result;
   size_t size,written;
   struct mtd_info_user mtd;
   struct erase_info_user erase;
   struct stat filestat;
   unsigned char src[BUFSIZE],dest[BUFSIZE];

   dev_fd = open (device,O_SYNC | O_RDWR);
   if(dev_fd < 0){
       return 2;
       }
   
   if (ioctl (dev_fd,MEMGETINFO,&mtd) < 0)
	 {
	printf("ioctl memgetinfo fail\n");
	return 3;
	 }
   
   fil_fd = open (filename,O_RDONLY);   
   if(fil_fd < 0){
      return 2;
       }
   
   if (fstat (fil_fd,&filestat) < 0)
	 {
	     printf("fstat vaild\n");
		return 1;
	 }

   if (filestat.st_size > mtd.size)
	 {
		return 1;
	 }


   erase.start = 0;
   erase.length = filestat.st_size & ~(mtd.erasesize - 1);
   if (filestat.st_size % mtd.erasesize) erase.length += mtd.erasesize;
		int blocks = erase.length / mtd.erasesize;
		erase.length = mtd.erasesize;
		for (i = 1; i <= blocks; i++)
		  {
			 if (ioctl (dev_fd,MEMERASE,&erase) < 0)
			   {
				return 1;;
			   }
			 erase.start += mtd.erasesize;
		  }
	
  	printf("Erased %u / %luk bytes\n",erase.length,filestat.st_size);


   size = filestat.st_size;
   i = BUFSIZE;
   written = 0;
   
   while (size)
	 {
		if (size < BUFSIZE) i = size;

		if(read (fil_fd,src,i) < 0){		 
			return 4;
		    }

		result = write (dev_fd,src,i);
		if (i != result)
		  {
			 if (result < 0){			    
				
			   }
			return 5;
		}
		written += i;
		size -= i;
	 }

   if (lseek (fil_fd,0L,SEEK_SET) < 0)
	 {
	     printf("lseek error\n");
		return 1;
	 }
   
   if (lseek (dev_fd,0L,SEEK_SET) < 0)
	 {	    
		return 1;
   	}
   
   size = filestat.st_size;
   i = BUFSIZE;
   written = 0;
 
   while (size)
	 {
		if (size < BUFSIZE) i = size;
		
		if(read (fil_fd,src,i) < 0){
		    return 4;
		    }

		
		if(read (dev_fd,dest,i) < 0){
		    return 4;
		    }
		
		if (memcmp (src,dest,i))
		  {
			 return -1;
		  }

		written += i;
		size -= i;
	 }
	close(fil_fd);
	close(dev_fd);
   return 0;
}

DBusMessage  *flash_erase_partition(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned int isFast = 0;
	int slot_id = 0;
	unsigned int reset_bitmask = 0x0;
	int ret = 0;
	int status;
	char *name = NULL;
        char temp_buf[100]={0};

	char *execute_result_buf = NULL;
	sem_syslog_event("flash erase!\n");
	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_STRING,&name,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
		}
	
	sprintf(temp_buf,"/dev/%s",name);
	sem_syslog_dbg("Execute command : %s\n",temp_buf);
//	status = system(temp_buf);
	status =  flash_erase_one_partition(temp_buf);

	if(status == -1){
		sem_syslog_warning("open device fail!\n");
		ret = 2;
	}
	else if(status == -2){
		sem_syslog_warning("unable to get MTD device\n");
		ret = 3;
	}
	else if(status == 1){
		sem_syslog_warning("unable to get MTD device\n");
		ret = 4;
	}
	else if(status == 0){
		sem_syslog_warning("succes\n");
		ret = 0;
	}
	else{
		ret = 1;
	}
	
    reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_INT32,
									 &ret);

	return reply;

}
DBusMessage *write_boot_to_flash(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	
	DBusMessage* reply = NULL;
		DBusMessageIter  iter;
		DBusError err;
		unsigned int isFast = 0;
		int slot_id = 0;
		unsigned int reset_bitmask = 0x0;
		int ret = 0;
		int status;
		char *name = NULL;
		char temp_buf[100]={0};
		char file_buf[100]={0};
		char mountblk[100] = {0};	
		int mstatus ;
		char *execute_result_buf = NULL;
		sem_syslog_event("flash write boot!\n");
		dbus_error_init(&err);
		
		if (!(dbus_message_get_args ( msg, &err,
												DBUS_TYPE_STRING,&name,
												DBUS_TYPE_INVALID))) {
												
				sem_syslog_warning("Unable to get input args ");
				if (dbus_error_is_set(&err)) {
					sem_syslog_warning("%s raised: %s",err.name,err.message);
					dbus_error_free(&err);
				}
				return NULL;
			}


		
	//	strcpy(mountblk,"sudo mount /blk");
	//	sprintf(temp_buf, "flashcp  /blk/%s  /dev/mtd1",name);
	sprintf(mountblk, "sor.sh  cpfromblk  %s  30",name);
	
	sprintf(file_buf, "/mnt/%s",name);
	sprintf(temp_buf, "/dev/mtd1");
		sem_syslog_dbg("Execute command : %s\n",temp_buf);
		//vty_out(vty, "starting erase\n");
	//	mstatus = system(mountblk);
	//	status = system(temp_buf);
	mstatus = system(mountblk);
	
	 status = flash_writ_boot(file_buf,temp_buf);
	if(status == 2){
			sem_syslog_warning("open device failed!\n");
			ret = 2;
		}
		else if(status == 3){
			sem_syslog_warning("unable to get MTD device\n");
			ret = 3;
		}
		else if(status == 4){
			sem_syslog_warning("read file failed\n");
			ret = 1;
		}
		else if(status == 5){
			sem_syslog_warning("writing failed\n");
			ret = 0;
		}
		else if(status == 0){
			sem_syslog_warning("success\n");
			ret = 0;
		}
		else if(status == 1){
			sem_syslog_warning("writing failed\n");
			ret = 1;
		}
		else if(status == -1){
			sem_syslog_warning("compare file failed\n");
			ret = -1;
			}
		else{
			ret = 1;
		}

		reply = dbus_message_new_method_return(msg);
		
		dbus_message_iter_init_append (reply, &iter);
		
		dbus_message_iter_append_basic (&iter,
										 DBUS_TYPE_INT32,
										 &ret);
	
		return reply;
	
}



DBusMessage *sem_dbus_delete_patch(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned int isFast = 0;
	int slot_id = 0;
//	unsigned int reset_bitmask = 0x0;
	int ret = 0;
	int fd_e;
	char c;
//	int status;
	char *name = NULL;
    char temp_buf[128]={0};

	sem_syslog_dbg("Dbus to sem for delete patch!\n");

	dbus_error_init(&err);

	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_STRING,&name,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	//sprintf(temp_buf,"vtysh -c \"apply patch %s\" >& /blk/patch.log",name);

	//delete /mnt/patch/**.sp first
	{
		memset(temp_buf,0,128);
		sprintf(temp_buf,"rm /mnt/patch/%s -f > /dev/null 2>/dev/null",name);
		
		sem_syslog_dbg("Execute command : %s\n",temp_buf);
		system(temp_buf);
	}
	memset(temp_buf,0,128);
	sprintf(temp_buf,"sudo /usr/bin/sor.sh ls patch/%s 20 | wc -l > /mnt/exist6",name);
	system(temp_buf);
	fd_e = open("/mnt/exist6", O_RDONLY);
	if (fd_e < 0)
	{
		sem_syslog_warning("open /mnt/exist6 error\n");
		return NULL;
	}
	read(fd_e, &c, 1);
	close(fd_e);
	if (c-'0')
	{
		memset(temp_buf,0,128);
		sprintf(temp_buf,"sor.sh rm patch/%s 20",name);
		sem_syslog_dbg("Execute command : %s\n",temp_buf);
		
		ret = system(temp_buf);	
		ret=WEXITSTATUS(ret) ;	
	}
	else
		ret = 6;

	reply = dbus_message_new_method_return(msg);
	
	dbus_message_iter_init_append (reply, &iter);
	
	dbus_message_iter_append_basic (&iter,
									 DBUS_TYPE_INT32,
									 &ret);
	return reply;

}
	
DBusMessage *sem_dbus_sem_send_trap(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	bm_op_args opt;
	int ret = 0;
	int i = 0;
	
    unsigned int num = 1;
    sem_syslog_dbg("**sem_dbus_sem_send_trap !\n");
	dbus_error_init(&err);

	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_UINT32, &num,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		sem_syslog_dbg("sem_dbus_sem_send_trap get arg failed and reset normal\n");
	}

    for(;i<num;i++)
    {
		sem_syslog_dbg("send the %d signal:\n",i+1);
        sem_send_signal_to_trap_debug();

    }
	
    
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter,
								 DBUS_TYPE_UINT32,
								 &ret);
	sem_syslog_dbg("**sem_dbus_sem_send_trap success!\n");
	return reply;
}


DBusMessage *sem_dbus_user_add_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;

	int ret = 0;
	int flag = 0;
	char *cmdstr = NULL;
	char cmd[256] = {0};
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
									DBUS_TYPE_UINT32, &flag,
									DBUS_TYPE_STRING, &cmdstr,
									DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}
	
	if(flag)
		sprintf(cmd,"sudo /opt/bin/vtysh -b -c \'%s\n\'",cmdstr);
	else
		sprintf(cmd,"sudo /opt/bin/vtysh -c \'%s\n\'",cmdstr);
		
	ret=system(cmd);
	
	
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	return reply;
}

DBusMessage *sem_dbus_user_del_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;

	int ret = 0;
	char *cmdstr = NULL;
	char cmd[256] = {0};
 
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_STRING, &cmdstr,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}
	sprintf(cmd,"sudo /opt/bin/vtysh -c \'%s\n\'",cmdstr);
	ret=system(cmd);
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	
	return reply;
}
DBusMessage *sem_dbus_user_role_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;

	int ret = 0;
	char *cmdstr = NULL;
	char cmd[256] = {0};
 
	dbus_error_init(&err);

	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_STRING, &cmdstr,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}
	sprintf(cmd,"vtysh -c \'%s\'\n",cmdstr);
	ret=system(cmd);
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	
	return reply;
}
#define VIEWGROUP "vtyview"
#define ADMINGROUP "vtyadmin"

DBusMessage *sem_dbus_user_show_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	dbus_error_init(&err);
	int i;
	int admin_num = 0;
	int view_num = 0;
	char admin_group[32][64]= {0};
	char view_group[32][64] = {0};
	char *username = NULL;
	struct group *admin_grentry = NULL;
	struct group *view_grentry = NULL;
	char *ptr=NULL;
	
	admin_grentry = getgrnam(ADMINGROUP);
	if (admin_grentry)
	{
		for(i=0;ptr=admin_grentry->gr_mem[i];i++)
		{
			strncpy(admin_group[admin_num++],ptr,strlen(ptr));
		}
	}
	
	view_grentry = getgrnam(VIEWGROUP);
	if (view_grentry)
	{
		for(i=0;ptr=view_grentry->gr_mem[i];i++)
		{
			strncpy(view_group[view_num++],ptr,strlen(ptr));
		}
			
	
	}
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&admin_num);
	for(i=0;i<admin_num;i++)
	{
		username=admin_group[i];
		dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING,&username);
	}
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&view_num);	
	for(i=0;i<view_num;i++)
	{
		username=view_group[i];
		dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING,&username);
	}
	return reply;
}

DBusMessage *sem_dbus_user_show_running(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	dbus_error_init(&err);
	int i;
	int admin_num = 0;
	int view_num = 0;
	int slot_id;
	char admin_group[32][256]= {0};
	char view_group[32][256] = {0};
	char *cmdstr = NULL;
	
	char *cmdadmin = "enable";
	char *cmdview= "view";
	char *ptr=NULL;
	struct group *grentry = NULL;
	struct passwd *passwd = NULL;
	struct spwd *spwd=NULL;
	char tmp[128]={0};
	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_UINT32, &slot_id,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}

	grentry = getgrnam(VIEWGROUP);
	if (grentry)
	{
		for(i=0;ptr=grentry->gr_mem[i];i++)
		{
			passwd=getpwnam(ptr);
			if(passwd)
			{
				spwd = getspnam(ptr);
				if(spwd)
				{
					if(strcmp(ptr,"admin"))
					{
						
						sprintf(tmp,"user add slot %d %s %s %s\n",slot_id,ptr,spwd->sp_pwdp,cmdview);
						strncpy(view_group[view_num++],tmp,strlen(tmp));
					}
					else
						continue;				
				}
			}
		}
	}	
	
	grentry = getgrnam(ADMINGROUP);
	if (grentry)
	{
		for(i=0;ptr=grentry->gr_mem[i];i++)
		{
			passwd=getpwnam(ptr);
			if(passwd)
			{
				spwd = getspnam(ptr);
				if(spwd)
				{
					if(strcmp(ptr,"admin"))
					{
						char tmp[81];
						sprintf(tmp,"user add slot %d %s %s %s\n",slot_id,ptr,spwd->sp_pwdp,cmdadmin);
						strncpy(admin_group[admin_num++],tmp,strlen(tmp));
					}
					else
						continue;				
				}
			}
		}
			
	}

	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&admin_num);
	
	for(i=0;i<admin_num;i++)
	{
		cmdstr=admin_group[i];
		dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING,&cmdstr);
	}
	
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&view_num);	
	for(i=0;i<view_num;i++)
	{
		cmdstr=view_group[i];
		dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING,&cmdstr);
	}
	return reply;
}
DBusMessage *sem_dbus_user_passwd_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;

	int ret = 0;
	char *cmdstr = NULL;
	char cmd[256] = {0};
 
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_STRING, &cmdstr,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}
	
	ret=system(cmdstr);
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	
	return reply;
}
DBusMessage *sem_dbus_user_is_exsit_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	char *cmdstr = NULL;
	int ret = 0;
	int opt = 0;
 	struct passwd *passwd = NULL;
 	
	struct group *group=NULL;
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_STRING, &cmdstr,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}

	passwd = getpwnam(cmdstr);
	if(passwd)
		ret = 1;
	else
		ret = 0;

	{
	int uid,i;

	group = getgrnam(ADMINGROUP);
	if(group && group->gr_mem )
	{
		for(i=0;group->gr_mem[i];i++)
			if(!strcmp(group->gr_mem[i],cmdstr))
				opt = 1;

	}
#if 0
	group = getgrnam(VIEWGROUP);
	if(group && group->gr_mem )
	{
		for(i=0;group->gr_mem[i];i++)
			if(!strcmp(group->gr_mem[i],name))
				return 0;

	}
	return -1;
#endif
}
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&opt);
	
	return reply;
}

DBusMessage *sem_dbus_download_img_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	char * cmdstr = NULL;
	int ret = 0;
 	char *urlstr = NULL;
	char *username = NULL;
	char *password = NULL;
	char *filename = NULL;
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,
		                        DBUS_TYPE_STRING, &cmdstr,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		return NULL;
	}
	/*
	sprintf(cmdstr,"downimg.sh %s %s %s %s;echo 1 > /var/run/download_slot_flag &",urlstr,username,password,filename);
  */
	ret = system(cmdstr);

	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	
	return reply;
}
DBusMessage *sem_dbus_download_cpy_config_slot(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	char path[256]={0};
	char cmdstr[512] = {0};
	char error_buf[128] = {0};
	char *error_str= NULL;
	int err_flag=0;
	int ret = 0;
	unsigned int host_id = 0;
	unsigned int local_id = 0;
 	char *urlstr = NULL;
	char *username = NULL;
	char *password = NULL;
	char *filename = NULL;

	
	dbus_error_init(&err);
	if (!(dbus_message_get_args(msg, &err,	
								DBUS_TYPE_UINT32,&local_id,
								DBUS_TYPE_UINT32,&host_id,
								DBUS_TYPE_STRING, &filename,
								DBUS_TYPE_INVALID)))
	{
		if (dbus_error_is_set(&err)) 
		{
			sem_syslog_dbg("%s raised: %s", err.name, err.message);
			dbus_error_free(&err);
		}
		
		return NULL;

	}
	
	memset(cmdstr,0,sizeof(cmdstr));
	sprintf(cmdstr, "sudo /opt/bin/vtysh -c \'configure terminal\ncopy %d %s to %d %s\'",local_id,filename, host_id,filename);						   	
	ret=system(cmdstr);
	if (-1 == ret)	
	{  
	   sprintf(error_buf,"system error!"); 
	   err_flag = 1;
	   goto out;
	}  
	else  
	{  
  
	   if (WIFEXITED(ret))	
	   {  
		   switch (WEXITSTATUS(ret))
		   {	
				case 0: 		 
					   	break;
				default:			
					sprintf(error_buf,"download error\n"); 
					err_flag = 1;
					goto out;
						
			}	
	   }  
	   else  
	   {  
		   sprintf(error_buf,"exit status = [%d]\n", WEXITSTATUS(ret));  
		   err_flag = 1;
		   goto out;
	   }  
    } 
    				   	
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&err_flag);
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&ret);
	return reply;
 	 
out:
	reply = dbus_message_new_method_return(msg);

	dbus_message_iter_init_append (reply, &iter);

	dbus_message_iter_append_basic (&iter, DBUS_TYPE_UINT32,&err_flag);
	dbus_message_iter_append_basic (&iter, DBUS_TYPE_STRING,&error_str);
	
	return reply;
}

#if 1

static char *dcli_rp_table[256] ;
static char * dcli_rtdsfield_table[256] ;
static struct rp_rule_node * g_rp_rule = NULL;
static struct rp_ip_route_node * g_rp_ip_route = NULL;

static void rtdsfield_initialize()
{
	char buf[512];
	FILE *fp;
	int size = 256;

	fp = fopen(RTFIELD_DATABASE_FILE, "r");
	if (!fp)
		return;
	while (fgets(buf, sizeof(buf), fp)) {
		char *p = buf;
		int id;
		char namebuf[512];

		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '#' || *p == '\n' || *p == 0)
			continue;
		if (sscanf(p, "0x%x %s\n", &id, namebuf) != 2 &&
		    sscanf(p, "0x%x %s #", &id, namebuf) != 2 &&
		    sscanf(p, "%d %s\n", &id, namebuf) != 2 &&
		    sscanf(p, "%d %s #", &id, namebuf) != 2) {
			return;
		}
		
		if (id<0 || id>size)
			continue;

		dcli_rtdsfield_table[id] = strdup(namebuf);
	}
	fclose(fp);
}

static void rt_tables_initialize()
{
	char buf[512];
	FILE *fp;
	int size = 256;
	int i;

	for(i=0;i<size;i++)
	{
		if(dcli_rp_table[i]){
			free(dcli_rp_table[i]);
			dcli_rp_table[i] = NULL;
		}
	}
	fp = fopen(RT_TABLE_DATABASE_FILE, "r");
	if (!fp)
		return;
	
	while (fgets(buf, sizeof(buf), fp)) {
		char *p = buf;
		int id;
		char namebuf[512];

		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '#' || *p == '\n' || *p == 0)
			continue;
		if (sscanf(p, "0x%x %s\n", &id, namebuf) != 2 &&
		    sscanf(p, "0x%x %s #", &id, namebuf) != 2 &&
		    sscanf(p, "%d %s\n", &id, namebuf) != 2 &&
		    sscanf(p, "%d %s #", &id, namebuf) != 2) {
		    fclose(fp);
			return;
		}
		
		if (id<0 || id>size)
			continue;

		dcli_rp_table[id] = strdup(namebuf);
	}
	fclose(fp);
}
static void rt_tables_write()
{
	FILE *fp;
	int size = 256;

	fp = fopen(RT_TABLE_DATABASE_FILE, "w");
	if (!fp)
		return;
	fprintf(fp,
			"#\n"
			"# reserved values\n"
			"#\n"
			"255 local\n"
			"254 main\n"
			"253 default\n"
			"0 unspec\n"
			"#\n"
			"# local\n"
			"#\n"
			"#1 inr.ruhep\n");
	for(size=1;size<253;size++)
	{
		if(rt_tables_entry_name(size)!=NULL)
			fprintf(fp,"%d %s\n",size,dcli_rp_table[size]);
	}
	fclose(fp);
}

struct rp_ip_route_node * rp_ip_route_pares(char *buf)
{
	char *p = buf;
	int sn,id;
	char namebuf[512];
	struct rp_ip_route_node *result;

	while (*p == ' ' || *p == '\t')
		p++;
	
	if (*p == '#' || *p == '\n' || *p == 0)
		return NULL;

	result = malloc(sizeof(*result));
	if(result == NULL)
		return NULL;
	memset(result,0,sizeof(*result));
	
	/*32756:  from all iif eth1-9 lookup _rp100*/
	if (sscanf(p, "%s via %s dev", result->prefix,result->dst) == 2)
	{
		if(!strcmp(result->prefix,"default"))
		{
			sprintf(result->prefix,"0.0.0.0/0");
		}
		else if(!strchr(result->prefix,'/'))
		{
			strcat(result->prefix,"/32");
		}
		result->type = 1;
	}
	/*32765:  from 100.0.0.0/8 lookup _rp100*/	
	else if(sscanf(p, "%s dev %s scope link", result->prefix,result->dst) == 2)
	{
		if(!strcmp(result->prefix,"default"))
		{
			sprintf(result->prefix,"0.0.0.0/0");
		}
		else if(!strchr(result->prefix,'/'))
		{
			strcat(result->prefix,"/32");
		}
		result->type = 2;
	}

	else
	{
		free(result);
		return NULL;
	}
	return result;
}

struct rp_rule_node * rp_rule_pares(char *buf)
{
	char *p = buf;
	int sn,id;
	char namebuf[32];
	struct rp_rule_node *result;

	while (*p == ' ' || *p == '\t')
		p++;
	
	if (*p == '#' || *p == '\n' || *p == 0)
		return NULL;

	result = malloc(sizeof(struct rp_rule_node));
	if(result == NULL)
		return NULL;
	memset(result,0,sizeof(struct rp_rule_node));
	memset(namebuf,0,32);
	/*32756:  from all iif eth1-9 lookup _rp100*/
	if (sscanf(p, "%d:	from all iif %s lookup %s", &result->sn,result->ifname, result->tablename) == 3)
	{
		result->type = 1;
	}
	/*32765:  from 100.0.0.0/8 lookup _rp100*/	
	else if(sscanf(p, "%d:	from %s lookup %s", &result->sn, result->prefix,result->tablename) == 3)
	{
			result->type = 2;
			if(!strcmp(result->prefix,"all"))
			{
				sprintf(result->prefix,"0.0.0.0/0");
			}
			else if(!strchr(result->prefix,'/'))
			{
				strcat(result->prefix,"/32");
			}
	}
	/*32757:  from all to 220.0.0.0/24 lookup _rp100*/
	else if(sscanf(p, "%d:	from all to %s lookup %s", &result->sn, result->prefix,result->tablename) == 3)
	{
		result->type = 3;
		if(!strchr(result->prefix,'/'))
		{
			strcat(result->prefix,"/32");
		}
	}
	/*32761:  from all tos 0x06 lookup _rp100 */
	else if(sscanf(p, "%d:	from all tos %s lookup %s", &result->sn,namebuf,result->tablename) == 3)
	{
		int i=0;
		
		for(i=0;i<256;i++)
		{
			if (dcli_rtdsfield_table[i] && (strcmp(dcli_rtdsfield_table[i], namebuf) == 0)) 
			{
				break;
			}
		}
		if(i<256){
			result->tos = i;
			result->type = 4;
		}
		else{
			free(result);
			return NULL;
		}
	}

	else
	{
		free(result);
		return NULL;
	}
	return result;
}

struct rp_ip_route_node *rp_ip_route_get(unsigned char rtb_id)
{
	FILE *fp;
	char cmdstr[CMD_STR_LEN]={0};
	char buf[512];
	struct rp_ip_route_node *ptmp;

	sprintf(cmdstr,"ip route list table %d",rtb_id);
	fp = popen(cmdstr, "r");
	if (!fp)
		return 	NULL;
	
	while (fgets(buf, sizeof(buf), fp)) {
		char *p = buf;

		ptmp = rp_ip_route_pares(p);
		if(!ptmp)
			continue;
		if(!g_rp_ip_route){
			g_rp_ip_route= ptmp;
			g_rp_ip_route->prev=g_rp_ip_route;
		}
		else
		{
			g_rp_ip_route->prev->next=ptmp;
			ptmp->prev=g_rp_ip_route->prev;
			g_rp_ip_route->prev=ptmp;
		}
	}

	pclose(fp);
	return g_rp_ip_route;
}

struct rp_rule_node *rp_rule_get(void)
{
	FILE *fp;
	char *cmdstr= "ip rule list";
	char buf[512];
	struct rp_rule_node *ptmp,*tmp;

	
	fp = popen(cmdstr, "r");
	if (!fp)
		return 	NULL;
	memset(buf,0,512);
	while (buf == fgets(buf, 512, fp)) {
		char *p = buf;

		while (*p == ' ' || *p == '\t')
			p++;
		
		if (*p == '#' || *p == '\n' || *p == 0)
			continue;

		
		ptmp = rp_rule_pares(p);
		if(!ptmp)
			continue;
		if(!g_rp_rule){
			g_rp_rule= ptmp;
			g_rp_rule->prev=g_rp_rule;
/*			ptmp->next = g_rp_rule;*/

		}
		else if(ptmp->type == 2)
		{

			u_char isset=1;
			tmp=g_rp_rule;
			while(tmp)
			{
				if( tmp->type == 2 &&
					!(strcmp(tmp->tablename ,ptmp->tablename) 
					||strcmp(tmp->prefix, ptmp->prefix)))
				{	
					free(ptmp);
					isset = 0;
					break;
				}
				tmp=tmp->next;
			}
			
			if(isset)
			{
				g_rp_rule->prev->next=ptmp;
				
				ptmp->prev=g_rp_rule->prev;
				g_rp_rule->prev=ptmp;
/*				ptmp->next = g_rp_rule;*/

			}

		}
		else
		{
			g_rp_rule->prev->next=ptmp;

			ptmp->prev=g_rp_rule->prev;
			g_rp_rule->prev=ptmp;
/*			ptmp->next = g_rp_rule;*/

		}
		memset(buf,0,512);
	}

	pclose(fp);
	return g_rp_rule;
}

void rp_ip_route_delete(unsigned char rtb_id)
{
	int i;	
	struct rp_ip_route_node * rtrn,*tmp;
	char cmdstr[CMD_STR_LEN]= {0};


	rtrn = rp_ip_route_get(rtb_id);
	if(rtrn == NULL)
	{
		return ;
	}
	
	while(rtrn )
	{
		switch(rtrn->type){
		case 1: /*32756:  from all iif eth1-9 lookup _rp100*/
			sprintf(cmdstr,"ip route del %s via %s table %d",rtrn->prefix,rtrn->dst,rtb_id);
			break;
		case 2: /*32765:  from 100.0.0.0/8 lookup _rp100*/	
			sprintf(cmdstr,"ip route del %s dev %s table %d",rtrn->prefix,rtrn->dst,rtb_id);
			break;
		default:
			break;
		}
		tmp = rtrn;
		rtrn=rtrn->next;
		free(tmp);
		system(cmdstr);
		system("ip route flush cache");
	}
	g_rp_ip_route = NULL;
	return;
}

void rp_rule_delete(unsigned char rtb_id)
{
	int i;	
	struct rp_rule_node * rtrn,*tmp;
	char cmdstr[CMD_STR_LEN]= {0};

	rt_tables_initialize();

	rtrn = rp_rule_get();
	if(rtrn == NULL)
	{
		return ;
	}
	
	while(rtrn )
	{
		if(rt_tables_entry_name(rtb_id)&&(strcmp(rtrn->tablename,rt_tables_entry_name(rtb_id)) == 0))
		{
			switch(rtrn->type){
				case 1: /*32756:  from all iif eth1-9 lookup _rp100*/
					sprintf(cmdstr,"ip rule del dev %s table %s",rtrn->ifname,rtrn->tablename);
					break;
				case 2: /*32765:  from 100.0.0.0/8 lookup _rp100*/	
					sprintf(cmdstr,"ip rule del from %s table %s",rtrn->prefix,rtrn->tablename);
					break;
				case 3: /*32757:  from all to 220.0.0.0/24 lookup _rp100*/
					sprintf(cmdstr,"ip rule del to %s table %s",rtrn->prefix,rtrn->tablename);
					break;
				case 4: /*32761:  from all tos 0x06 lookup _rp100 */
					sprintf(cmdstr,"ip rule del tos %x table %s",rtrn->tos,rtrn->tablename);
					break;
				default:
					break;
				}
			system(cmdstr);
			system("ip route flush cache");
		}
		tmp = rtrn;
		rtrn=rtrn->next;
		free(tmp);
	}

	g_rp_rule = NULL;
	return;
}


/****************************************************
*cmd:
*	0 delete
*	1 add or modify
*
************************************************************/
static int rt_tables_entry_changed(int cmd,unsigned char rtb_id )
{
	char rtb_name[32];
	char *tmp_str;

	if(rtb_id==0 || rtb_id>32 )
		return 0;

	if(dcli_rp_table[rtb_id] != NULL)
	{
		free(dcli_rp_table[rtb_id]);
		dcli_rp_table[rtb_id]=NULL;
	}
	if(cmd == 1)/*add or modify*/
	{
	//	tmp_str=RTB_NAME(rtb_name,rtb_id);//
		//#define RTB_NAME(_buf,_rtb_id) sprintf(_buf,"_rp%u",_rtb_id)
		sprintf(rtb_name,"_rp%u",rtb_id);
		dcli_rp_table[rtb_id] = strdup(rtb_name);
	}
	return 1;
}


int set_route_policy(unsigned char rtb_id)
{	
	/*if(!IS_RTB_ID(rtb_id))
	{
		sem_syslog_warning("Error policy id %s\n",rtb_id);
		return -1;
	}*/
	rt_tables_initialize();
	if(rt_tables_entry_name(rtb_id))
	{
	//	g_rtb_id = (unsigned char)rtb_id;
	//	vty->node = ROUTE_POLICY_NODE;
	//	vty->index = &g_rtb_id;
		return 0;
	}
	if(rt_tables_entry_changed(1,rtb_id))
	{
		rt_tables_write();
	//	g_rtb_id = (unsigned char)rtb_id;
	//	vty->index = &g_rtb_id;
	//	vty->node = ROUTE_POLICY_NODE;
		return 0;
	}
	else
	{
		sem_syslog_warning("Can't create route policy %s\n",rtb_id);
		return -1;
	}
}
int del_route_policy(unsigned char rtb_id)
{	
	/*if(!IS_RTB_ID(rtb_id))
	{
		sem_syslog_warning("Error policy id %s\n",rtb_id);
		return -1;
	}*/
	
	rt_tables_initialize();
	
	rp_ip_route_delete(rtb_id);
	rp_rule_delete(rtb_id);
	
	if(!rt_tables_entry_name(rtb_id))
	{
		sem_syslog_warning("The policy of id %s isn't exist\n",rtb_id);
		return 0;
	}
	if(rt_tables_entry_changed(0,rtb_id))
	{
		rt_tables_write();
		return 0;
	}
	else
	{
		sem_syslog_warning("Can't delete route policy %s\n",rtb_id);
		return -1;
	}
}


int route_policy_ip(unsigned char rtb_id, char *action, char *direction, char *ip)
{	
    char cmdstr[128] ={0};
	char *rtb_name=NULL;
	
	if(!(rtb_name = rt_tables_entry_name(rtb_id)))
	{
		sem_syslog_warning("Get route policy error, please check it: %s\n",rtb_id);
		return -1;
	}
	sprintf(cmdstr,"ip rule %s %s %s table %s",
		strncmp(action,"a",1)?"del":"add",
		strncmp(direction,"f",1)?"to":"from",ip,
		rtb_name);
	
	system(cmdstr);
	system("ip route flush cache");/*ip rule show*/
	return 0;
	
}

int route_policy_tos(unsigned char rtb_id, char *action, int tos)
{	
    char cmdstr[128] ={0};
	char *rtb_name=NULL;
	
	if(!(rtb_name = rt_tables_entry_name(rtb_id)))
	{
		sem_syslog_warning("Get route policy error, please check it: %s\n",rtb_id);
		return -1;
	}
	sprintf(cmdstr,"ip rule %s tos %x table %s",
		strncmp(action,"a",1)?"del":"add",tos,rtb_name);
	
	system(cmdstr);
	system("ip route flush cache");/*ip rule show*/
	return 0;
	
}

int route_policy_interface(unsigned char rtb_id, char *action, char *ifname)
{	
    char cmdstr[128] ={0};
	char *rtb_name=NULL;
	
	if(!(rtb_name = rt_tables_entry_name(rtb_id)))
	{
		sem_syslog_warning("Get route policy error, please check it: %s\n",rtb_id);
		return -1;
	}
	sprintf(cmdstr,"ip rule %s dev %s table %s",
		strncmp(action,"a",1)?"del":"add",ifname,rtb_name);
	
	system(cmdstr);
	system("ip route flush cache");
	
	return 0;
	
}

int ip_route_policy_add(unsigned char rtb_id, int action, char *dst_str, char *gw_str)
{	
    char cmdstr[128] ={0};
	char *rtb_name=NULL;
	
	if(!(rtb_name = rt_tables_entry_name(rtb_id)))
	{
		sem_syslog_warning("Get route policy error, please check it: %s\n",rtb_id);
		return -1;
	}

	/*1 : gw is ip adress ; 0: gw is interface*/
	if(action==1)
		sprintf(cmdstr,"ip route add %s via %s table %s",dst_str,gw_str,rtb_name);
	else
		sprintf(cmdstr,"ip route add %s dev %s table %s",dst_str,gw_str,rtb_name);

	system(cmdstr);
	
	return 0;
	
}

int ip_route_policy_del(unsigned char rtb_id, int action, char *dst_str, char *gw_str)
{	
    char cmdstr[128] ={0};
	char *rtb_name=NULL;
	
	if(!(rtb_name = rt_tables_entry_name(rtb_id)))
	{
		sem_syslog_warning("Get route policy error, please check it: %s\n",rtb_id);
		return -1;
	}

	/*1 : gw is ip adress ; 0: gw is interface*/
	if(action==1)
		sprintf(cmdstr,"ip route del %s via %s table %s",dst_str,gw_str,rtb_name);
	else
		sprintf(cmdstr,"ip route del %s dev %s table %s",dst_str,gw_str,rtb_name);

	system(cmdstr);
	
	return 0;
	
}

static void ip_rule_show(unsigned char rtb_id,const char* rtb_name,char *conf)
{
	int i;	
	struct rp_rule_node * rtrn,*tmp;

	if(rtb_name)/*show */
	{
		rtrn = rp_rule_get();
		if(rtrn == NULL)
		{
			sprintf(conf,"%sThe system route policy rule %d is NULL.\n",conf,rtb_id);
			return ;
		}
		
		sprintf(conf,"%sRoute policy %d ip rule :\n",conf,rtb_id);
		while(rtrn )
		{
			if(strcmp(rtrn->tablename,rtb_name) == 0)
			{
				switch(rtrn->type){
					case 1: /*32756:  from all iif eth1-9 lookup _rp100*/
						sprintf(conf,"%s%8d: from all IIF %s\n",conf,rtrn->sn,rtrn->ifname);
						break;
					case 2: /*32765:  from 100.0.0.0/8 lookup _rp100*/	
						sprintf(conf,"%s%8d: from %s \n",conf,rtrn->sn,rtrn->prefix);
						break;
					case 3: /*32757:  from all to 220.0.0.0/24 lookup _rp100*/
						sprintf(conf,"%s%8d: from all to %s \n",conf,rtrn->sn,rtrn->prefix);
						break;
					case 4: /*32761:  from all tos 0x06 lookup _rp100 */
						sprintf(conf,"%s%8d: from all tos %d \n",conf,rtrn->sn,rtrn->tos);
						break;
					default:
						break;
					}
			}
			tmp = rtrn;
			rtrn=rtrn->next;
			free(tmp);
		
		}
		g_rp_rule = NULL;
	}
	sprintf(conf,"%s\n",conf);
	return;
}

static void ip_route_show(unsigned char rtb_id,const char* rtb_name, char *conf)
{
	int i;	
	struct rp_ip_route_node * rtrn,*tmp;

	if(rtb_name)/*show */
	{
		rtrn = rp_ip_route_get(rtb_id);
		if(rtrn == NULL)
		{
			sprintf(conf,"%sThe system policy route rule %d is none.\n",conf,rtb_id);
			return ;
		}
		
		sprintf(conf,"%sRoute policy %d ip route \n",conf,rtb_id);
		while(rtrn )
		{
			switch(rtrn->type){
			case 1: /*32756:  from all iif eth1-9 lookup _rp100*/
				sprintf(conf,"%s%s via %s \n",conf,rtrn->prefix,rtrn->dst);
				break;
			case 2: /*32765:  from 100.0.0.0/8 lookup _rp100*/	
				sprintf(conf,"%s%s dev %s scope link \n",conf,rtrn->prefix,rtrn->dst);
				break;
			default:
				break;
			}
			tmp = rtrn;
			rtrn=rtrn->next;
			free(tmp);
		
		}
		g_rp_ip_route = NULL;
	}
	sprintf(conf,"%s\n",conf);
	return;
}

int route_policy_show(unsigned char rtb_id, char *conf)
{	

	rt_tables_initialize();
	if(rtb_id != 0)/*some one*/
	{
		if(rt_tables_entry_name(rtb_id))
			ip_rule_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
		else{
			sprintf(conf,"The route policy %d doesn't create.\n",rtb_id);
			return -2;
		}
	}
	else /*all*/
	{
		for(rtb_id = 1; rtb_id<=32;rtb_id++)
		{			
			if(rt_tables_entry_name(rtb_id))
				ip_rule_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
		}
	}
	return 0;

}


int ip_route_policy_show(unsigned char rtb_id, char *conf)
{	

	rt_tables_initialize();
	if(rtb_id != 0)/*some one*/
	{
		if(rt_tables_entry_name(rtb_id))
			ip_route_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
		else{
			sprintf(conf,"The route policy %d doesn't create.\n",rtb_id);
			return -2;
		}
	}
	else /*all*/
	{
		for(rtb_id = 1; rtb_id<=32;rtb_id++)
		{			
			if(rt_tables_entry_name(rtb_id))
				ip_route_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
		}
	}
	return 0;

}

int route_policy_show_all(char *conf)
{
	unsigned char rtb_id;

	rt_tables_initialize(); 
	
	for(rtb_id = 1; rtb_id<=32;rtb_id++)
	{
		if(rt_tables_entry_name(rtb_id) )
		{
			ip_rule_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
			sprintf(conf,"%s\n",conf);
			ip_route_show(rtb_id,rt_tables_entry_name(rtb_id),conf);
			sprintf(conf,"%s\n\n",conf);
		}
		
	}

	return 0;
}

int route_policy_show_running(char *conf)
{
//	char _tmpstr[256];
	unsigned char rtb_id;

	//memset(_tmpstr,0,256);
	//sprintf(_tmpstr,BUILDING_MOUDLE,"ROUTE POLICY");
//	vtysh_add_show_string(_tmpstr);

	rt_tables_initialize(); 
	
	for(rtb_id = 1; rtb_id<=32;rtb_id++)
	{			
		if(rt_tables_entry_name(rtb_id)){
			struct rp_ip_route_node * ip_rtrn,*ip_rtrn_tmp;
			struct rp_rule_node * rule_rtrn, *rule_rtrn_tmp;

			sprintf(conf,"%s\nroute policy %d\n",conf,rtb_id);
			//vtysh_add_show_string(_tmpstr);

			rule_rtrn = rp_rule_get();
			while(rule_rtrn )
			{
				if(strcmp(rule_rtrn->tablename,rt_tables_entry_name(rtb_id)) == 0)
				{
					switch(rule_rtrn->type){
						case 1: /*32756:  from all iif eth1-9 lookup _rp100*/
							sprintf(conf,"%s policy rule add interface %s\n",conf,rule_rtrn->ifname);
							//vtysh_add_show_string(_tmpstr);
							break;
						case 2: /*32765:  from 100.0.0.0/8 lookup _rp100*/	
							sprintf(conf,"%s policy rule add from %s\n",conf,rule_rtrn->prefix);
							//vtysh_add_show_string(_tmpstr);
							break;
						case 3: /*32757:  from all to 220.0.0.0/24 lookup _rp100*/
							sprintf(conf,"%s policy rule add to %s\n",conf,rule_rtrn->prefix);
							//vtysh_add_show_string(_tmpstr);
							break;
						case 4: /*32761:  from all tos 0x06 lookup _rp100 */
							sprintf(conf,"%s policy rule add tos %d\n",conf,rule_rtrn->tos);
							//vtysh_add_show_string(_tmpstr);
							break;
						default:
							break;
						}
				}
				rule_rtrn_tmp = rule_rtrn;
				rule_rtrn=rule_rtrn->next;
				free(rule_rtrn_tmp);
			}
			g_rp_rule = NULL;
			

			ip_rtrn = rp_ip_route_get(rtb_id);
			while(ip_rtrn)
			{
				sprintf(conf,"%s ip route %s %s\n",conf,ip_rtrn->prefix,ip_rtrn->dst);
				//vtysh_add_show_string(_tmpstr);
				
				ip_rtrn_tmp = ip_rtrn;
				ip_rtrn=ip_rtrn->next;
				free(ip_rtrn_tmp);				
			}
			g_rp_ip_route = NULL;
		

			sprintf(conf,"%s exit\n",conf);

		}
		
	}
	
	sprintf(conf,"%s\n",conf);

	return 0;

}



DBusMessage *sem_dbus_set_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	/*sem_syslog_warning(" %s : line %d\n",__func__,__LINE__);*/

	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	/*sem_syslog_warning(" %s : line %d\n",__func__,__LINE__);*/
	ret = set_route_policy(rtb_id);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}

DBusMessage *sem_dbus_del_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);

	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = del_route_policy(rtb_id);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}


DBusMessage *sem_dbus_route_policy_ip(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	
	char *action = NULL;
	char *direction = NULL;
	char *ip = NULL;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
		                                    DBUS_TYPE_STRING,&action,
		                                    DBUS_TYPE_STRING,&direction,
		                                    DBUS_TYPE_STRING,&ip,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = route_policy_ip(rtb_id,action,direction,ip);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}


DBusMessage *sem_dbus_route_policy_tos(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	int tos = 0;
	
	char *action = NULL;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
		                                    DBUS_TYPE_STRING,&action,
		                                    DBUS_TYPE_INT32,&tos,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = route_policy_tos(rtb_id,action,tos);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}


DBusMessage *sem_dbus_route_policy_interface(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	char *ifname = NULL;
	
	char *action = NULL;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
		                                    DBUS_TYPE_STRING,&action,
		                                    DBUS_TYPE_STRING,&ifname,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = route_policy_interface(rtb_id,action,ifname);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}


DBusMessage *sem_dbus_add_ip_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	int action = -1;
	char *dst_str = NULL;
	char *gw_str = NULL;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
		                                    DBUS_TYPE_INT32,&action,
		                                    DBUS_TYPE_STRING,&dst_str,
		                                    DBUS_TYPE_STRING,&gw_str,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = ip_route_policy_add(rtb_id,action,dst_str,gw_str);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}


DBusMessage *sem_dbus_del_ip_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	int action = -1;
	char *dst_str = NULL;
	char *gw_str = NULL;
	
	sem_syslog_dbg("Dbus to sem for set route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
		                                    DBUS_TYPE_INT32,&action,
		                                    DBUS_TYPE_STRING,&dst_str,
		                                    DBUS_TYPE_STRING,&gw_str,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			return NULL;
	}
	
	ret = ip_route_policy_del(rtb_id,action,dst_str,gw_str);
	
	reply = dbus_message_new_method_return(msg);
	dbus_message_iter_init_append (reply, &iter);
	dbus_message_iter_append_basic (&iter,DBUS_TYPE_INT32,&ret);

	return reply;

}

#define ROUTE_CONF_SIZE (1024 * 1024)
#define	rc_calloc(cnt, sz)	calloc((cnt), (sz))
#define	rc_free(p)		free((p))

DBusMessage *sem_dbus_show_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	char *conf = rc_calloc(1, ROUTE_CONF_SIZE);
	
	if(!conf) {
		sem_syslog_warning("out of memory\n");
		goto failed;
	}
	
	sem_syslog_dbg("Dbus to sem for show route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			goto failed;
	}
	/*rtb_id = 0 : all info; relse is someone.*/
	ret = route_policy_show(rtb_id,conf);
	reply = dbus_message_new_method_return(msg);
	dbus_message_append_args(reply,
									DBUS_TYPE_INT32, &ret,
									DBUS_TYPE_STRING, &conf,
									DBUS_TYPE_INVALID);

	done:
		if(conf)
			rc_free(conf);
		return reply;
	
	failed:
		reply = NULL;
		goto done;

}

DBusMessage *sem_dbus_show_ip_route_policy(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	char *conf = rc_calloc(1, ROUTE_CONF_SIZE);
	
	if(!conf) {
		sem_syslog_warning("out of memory\n");
		goto failed;
	}
	
	sem_syslog_dbg("Dbus to sem for show route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			goto failed;
	}
	/*rtb_id = 0 : all info; relse is someone.*/
	ret = ip_route_policy_show(rtb_id,conf);
	reply = dbus_message_new_method_return(msg);
	dbus_message_append_args(reply,
									DBUS_TYPE_INT32, &ret,
									DBUS_TYPE_STRING, &conf,
									DBUS_TYPE_INVALID);

	done:
		if(conf)
			rc_free(conf);
		return reply;
	
	failed:
		reply = NULL;
		goto done;

}


DBusMessage *sem_dbus_show_route_policy_all(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	char *conf = rc_calloc(1, ROUTE_CONF_SIZE);
	
	if(!conf) {
		sem_syslog_warning("out of memory\n");
		goto failed;
	}
	
	sem_syslog_dbg("Dbus to sem for show route policy.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			goto failed;
	}
	/*rtb_id = 0 : all info; relse is someone.*/
	ret = route_policy_show_all(conf);
	reply = dbus_message_new_method_return(msg);
	dbus_message_append_args(reply,
									DBUS_TYPE_INT32, &ret,
									DBUS_TYPE_STRING, &conf,
									DBUS_TYPE_INVALID);

	done:
		if(conf)
			rc_free(conf);
		return reply;
	
	failed:
		reply = NULL;
		goto done;

}


DBusMessage *sem_dbus_route_policy_show_running(DBusConnection *conn, DBusMessage *msg, void *user_data)
{
	DBusMessage* reply = NULL;
	DBusMessageIter  iter;
	DBusError err;
	unsigned char rtb_id = 0;
	int ret = 0;
	char *conf = rc_calloc(1, ROUTE_CONF_SIZE);
	
	if(!conf) {
		sem_syslog_warning("out of memory\n");
		goto failed;
	}
	
	sem_syslog_dbg("Dbus to sem for show route running.\n");

	dbus_error_init(&err);
	
	if (!(dbus_message_get_args ( msg, &err,
		                                    DBUS_TYPE_BYTE,&rtb_id,
											DBUS_TYPE_INVALID))) {
											
			sem_syslog_warning("Unable to get input args ");
			if (dbus_error_is_set(&err)) {
				sem_syslog_warning("%s raised: %s",err.name,err.message);
				dbus_error_free(&err);
			}
			goto failed;
	}
	/*rtb_id = 0 : all info; relse is someone.*/
	ret = route_policy_show_running(conf);
	reply = dbus_message_new_method_return(msg);
	dbus_message_append_args(reply,
									DBUS_TYPE_INT32, &ret,
									DBUS_TYPE_STRING, &conf,
									DBUS_TYPE_INVALID);

	done:
		if(conf)
			rc_free(conf);
		return reply;
	
	failed:
		reply = NULL;
		goto done;

}

#endif
#ifdef __cplusplus
extern "C"
}
#endif

