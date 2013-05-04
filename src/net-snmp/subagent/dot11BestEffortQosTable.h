/*
 * Note: this file originally auto-generated by mib2c using
 *  : mib2c.iterate.conf,v 5.14.2.2 2005/05/09 08:13:01 dts12 Exp $
 */
#ifndef DOT11BESTEFFORTQOSTABLE_H
#define DOT11BESTEFFORTQOSTABLE_H

/* function declarations */
void init_dot11BestEffortQosTable(void);
void initialize_table_dot11BestEffortQosTable(void);
Netsnmp_Node_Handler dot11BestEffortQosTable_handler;
Netsnmp_First_Data_Point  dot11BestEffortQosTable_get_first_data_point;
Netsnmp_Next_Data_Point   dot11BestEffortQosTable_get_next_data_point;

/* column number definitions for table dot11BestEffortQosTable */
       #define COLUMN_BESTEFFORTWTPID		1
       #define COLUMN_BESTEFFORTRADIOLOCALID		2
       #define COLUMN_BESTEFFORTMAXSVCCNT		3
       #define COLUMN_BESTEFFORTSVCBW		4
       #define COLUMN_BESTEFFORTSVCBWPERCENT		5
       #define COLUMN_BESTEFFORTISUSEWREDALG		6
       #define COLUMN_BESTEFFORTISUSETRAFFICSHAPING		7
	   #define BESTEFFORTMIN                 COLUMN_BESTEFFORTWTPID         
	   #define BESTEFFORTMAX                 COLUMN_BESTEFFORTISUSETRAFFICSHAPING
#endif /* DOT11BESTEFFORTQOSTABLE_H */
