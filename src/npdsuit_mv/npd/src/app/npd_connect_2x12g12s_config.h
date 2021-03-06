
/*8610*/
asic_cscd_bport_t asic_2x12g12s_8610_cscd_bports[8][16]= 
{
    {   
        {26,5,0,0,0}, /*slot 1 {cscd port, master 0&1, asic lion 0&1, bport, trunk_id}*/
    	{27,6,0,0,0}
    },
    {
        {26,5,0,26,0},/*slot 2*/
    	{27,6,0,26,0}
    },
    {
        {26,5,0,32,0},/*slot 3*/
    	{27,6,0,32,0}
    },
    {
        {26,5,0,58,0},/*slot 4*/
    	{27,6,0,58,0}
    },
    {
        {26,5,0,52,0},/*slot 7*/
    	{27,6,0,52,0}
    },
    {
        {26,5,0,38,0},/*slot 8*/
    	{27,6,0,38,0}
    },
    {
        {26,5,0,20,0},/*slot 9*/
    	{27,6,0,20,0}
    },
    {
        {26,5,0,6,0},/*slot 10*/
    	{27,6,0,6,0}
    }
};
/*8800*/
asic_cscd_bport_t asic_2x12g12s_8800_cscd_bports[14][16]=
{
    {   
        {26,7,0,0,0}, /*slot 1 {cscd port, master 0&1, asic lion 0&1, bport, TRUNK_ID}*/
    	{27,8,0,0,0}
    },
    {
        {26,7,0,8,0}, /*slot 2*/
    	{27,8,0,8,0}
    },
    {
        {26,7,0,22,0}, /*slot 3*/
    	{27,8,0,22,0}
    },
    {
        {26,7,0,32,0}, /*slot 4*/
    	{27,8,0,32,0}
    },
    {
        {26,7,0,40,0}, /*slot 5*/
    	{27,8,0,40,0}
    },
    {
        {26,7,0,54,0}, /*slot 6*/
    	{27,8,0,54,0}
    },
    {
        {26,7,0,50,0}, /*slot 9*/
    	{27,8,0,50,0}
    },
    {
        {26,7,0,58,0}, /*slot 10*/
    	{27,8,0,58,0}
    },
    {
        {26,7,0,36,0}, /*slot 11*/
    	{27,8,0,36,0}
    },
    {
        {26,7,0,18,0}, /*slot 12*/
    	{27,8,0,18,0}
    },
    {
        {26,7,0,26,0}, /*slot 13*/
    	{27,8,0,26,0}
    },
    {
        {26,7,0,4,0}, /*slot 14*/
    	{27,8,0,4,0}
    }
};


asic_board_cscd_bport_t asic_board_2x12g12s_cscd_bports={
    .board_type = 0,
	.slot_id = -1,
	.asic_cscd_port_cnt = 2
	/*.asic_cscd_bports =  */
};
