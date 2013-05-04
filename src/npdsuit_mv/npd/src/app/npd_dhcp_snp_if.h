#ifndef __NPD_DHCP_SNP_IF_H__
#define __NPD_DHCP_SNP_IF_H__


/*********************************************************
*	macro define													*
**********************************************************/

/*********************************************************
*	struct define													*
**********************************************************/

/*********************************************************
*	function declare												*
**********************************************************/
	

/**********************************************************************************
 *npd_dhcp_snp_get_slot_port_from_eth_index ()
 *
 *	DESCRIPTION:
 *
 *	INPUTS:
 *		unsigned int eth_g_index
 *
 *	OUTPUTS:
 *		unsigned int *slot_no,
 *		unsigned int *port_no
 *
 *	RETURN VALUE:
 *
 ***********************************************************************************/
void npd_dhcp_snp_get_slot_port_from_eth_index
(
	unsigned int eth_g_index,
	unsigned int *slot_no,
	unsigned int *port_no
);

/*********************************************************
*	extern Functions												*
**********************************************************/

#endif




