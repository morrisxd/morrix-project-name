	//########### SERDES 0 Init
	
	//# Release the Power On Reset
	//# Set Tx/Rx Data Rate to 1
	mm(MAP_SERDES0_IO_CTRL_1, 0x5001);

	mm(MAP_SERDES0_CMU_001, 0x9641);       //CMU add 0x001
	mm(MAP_SERDES0_CMU_002, 0x2342);       //CMU add 0x002
	mm(MAP_SERDES0_LANE_303, 0x0B0F);       //LANE add 0x303 - rxfl_ldls
	mm(MAP_SERDES0_GPON_600, 0x808A);       //GPON add 0x600
	mm(MAP_SERDES0_PIPE_700, 0x5010);
	mm(MAP_SERDES0_GPON_604, 0x2000); 

	//# Release CMU Reset
	mm(MAP_SERDES0_IO_CTRL_0, 0x00000002);
	//# Release Reset
	mm(MAP_SERDES0_IO_CTRL_1, 0x00015801);

	//########### SERDES 1 Init
	
	//# Release the Power On Reset
	//# Set Tx/Rx Data Rate to 1
	mm(MAP_SERDES1_IO_CTRL_1, 0x5001);
						
	mm(MAP_SERDES1_CMU_001, 0x9641);       //CMU add 0x001
	mm(MAP_SERDES1_CMU_002, 0x2342);       //CMU add 0x002
	mm(MAP_SERDES1_LANE_303, 0x0B0F);       //LANE add 0x303 - rxfl_ldls
	mm(MAP_SERDES1_GPON_600, 0x808A);       //GPON add 0x600
	mm(MAP_SERDES1_PIPE_700, 0x5010);
	mm(MAP_SERDES1_GPON_604, 0x2000); 
	
	//# Release CMU Reset
	mm(MAP_SERDES1_IO_CTRL_0, 0x00000002);
	//# Release Reset
	mm(MAP_SERDES1_IO_CTRL_1, 0x00015801);


	
	//########### SERDES 2 Init - 1 GigE
	mm(MAP_SERDES2_IO_CTRL_0, 0x0040000); //tx data rate 01
	
	mm(MAP_SERDES2_IO_CTRL_1, 0x1600000); //rx_data_rate 01, tx_ex, lock_en
	mm(MAP_SERDES2_IO_CTRL_2, 0x1600000); //rx_data_rate 01, tx_ex, lock_en
	mm(MAP_SERDES2_IO_CTRL_3, 0x1600000); //rx_data_rate 01, tx_ex, lock_en
	mm(MAP_SERDES2_IO_CTRL_4, 0x1600000); //rx_data_rate 01, tx_ex, lock_en
	mm(MAP_SERDES2_IO_CTRL_0, 0x0050000); //set porn bit
        mm(MAP_SERDES2_IO_CTRL_1, 0x5600000);
        mm(MAP_SERDES2_IO_CTRL_2, 0x5600000);
	mm(MAP_SERDES2_IO_CTRL_3, 0x5600000);
	mm(MAP_SERDES2_IO_CTRL_4, 0x5600000);
	
	//########### SERDES 3 Init - 1 GigE


	//########### SERDES 4 Init - 1 GigE

{
	//#####################################
	//# SERDES 0 Init - effective 2.5G
	//####################################
{
	//#####################################
	//# SERDES 1 Init - effective 2.5G
	//####################################
	//# Release the Power On Reset
	//# Set Tx/Rx Data Rate to 3
	mm(MAP_SERDES1_IO_CTRL_1, 0xf001);

	//#configure for 2.5G effective - T =3.2ns
	mm(0x00408980,  0x00003882);       //LANE add 0x300, rxvcofr = 2b10,rxprediv4_ena =1b1 ,rxfl_refdiv 2d3
	mm(0x00408804,  0x00009ed5);       //CMU add 0x001 ,txfl_ldls= 6d30,txpll_refdiv2_ena=1b1,txprediv4_ena=1b1,txfl_refdiv_ena=2d3
	mm(0x00408808,  0x00005e52);       //CMU add 0x002 ,txdivnsel=gd23,txvcofr=2b10,c1_sel=2b01,r1_sel=3b010
	mm(0x0040880c,  0x0000d900);       //CMU add 0x003 ,p_cp_sel=2b10,i_cp_sel=2b11
	mm(0x0040898c,  0x00000f0f);       //LANE add 0x303 , rxfl_ldls=6d30
	mm(0x00408818,  0x00008400);       //CMU add 0x006 , txvcodiv_i2c=3b100
	mm(0x00408B00,  0x0000808a);       //GPON add 0x600,lnafe_pcs_rate=1b1,syn_align_hold=1b0
	mm(0x00408B04,  0x00008300);       //GPON add 0x601,gpon_en=1b0
	
	//# Release CMU Reset
	mm(MAP_SERDES1_IO_CTRL_0, 0x2);
	//# Release Reset
	mm(MAP_SERDES1_IO_CTRL_1, 0x1f801);

	//######################################################
	//# Init SerDes 2 to 2.5Gig Quad Lane with lock enable 
	//#####################################################
	
	mm(MAP_SERDES2_IO_CTRL_0, 0x000d0000);
	mm(MAP_SERDES2_IO_CTRL_1, 0x07600000);
	mm(MAP_SERDES2_IO_CTRL_2, 0x07600000);
	mm(MAP_SERDES2_IO_CTRL_3, 0x07600000);
	mm(MAP_SERDES2_IO_CTRL_4, 0x07600000);
	
	// serdes2 internal registers init for effective 2.5Giga per lane
	
	mm(0x00408c08, 0x00005e0a);   // CMU Register address 0x002
	mm(0x00408c04, 0x00009ed5);   // CMU Register address 0x001
	mm(0x00408c00, 0x0000009c);   // CMU Register address 0x000
	mm(0x00408c0c, 0x0000aa00);   // CMU Register address 0x003
	
	mm(0x00408c90, 0x0000830c);   // lane 0 address 0x04
	mm(0x00408c94, 0x00008000);   // lane 0 address 0x05
	mm(0x00408c80, 0x00003882);   // lane 0 address 0x00
	mm(0x00408c84, 0x0000726b);   // lane 0 address 0x01
	mm(0x00408c8c, 0x00000f14);   // lane 0 address 0x03

	mm(0x00408d00, 0x00003882);   // lane 1 address 0x00
	mm(0x00408d04, 0x0000726b);   // lane 1 address 0x01
	mm(0x00408d0c, 0x00000f14);   // lane 1 address 0x03
	mm(0x00408d10, 0x0000830c);   // lane 1 address 0x04
	mm(0x00408d14, 0x00008000);   // lane 1 address 0x05

	mm(0x00408d80, 0x00003882);   // lane 2 address 0x00
	mm(0x00408d84, 0x0000726b);   // lane 2 address 0x01
	mm(0x00408d8c, 0x00000f14);   // lane 2 address 0x03
	mm(0x00408d90, 0x0000830c);   // lane 2 address 0x04
	mm(0x00408d94, 0x00008000);   // lane 2 address 0x05

	mm(0x00408e00, 0x00003882);   // lane 3 address 0x00
	mm(0x00408e04, 0x0000726b);   // lane 3 address 0x01
	mm(0x00408e0c, 0x00000f14);   // lane 3 address 0x03
	mm(0x00408e10, 0x0000830c);   // lane 3 address 0x04
	mm(0x00408e14, 0x00008000);   // lane 3 address 0x05
	
	//######################################################
	//# Init SerDes 3 to 2.5Gig Quad Lane with lock enable
	//#####################################################
	
	//######################################################
	//# Init SerDes 4 to 2.5Gig Quad Lane with lock enable
	//#####################################################

/* polling */
	
mc MAP_SERDES3_IO_STAT 0x00001111 0x00001111

mc MAP_SERDES2_IO_STAT 0x00001111 0x00001111

mc MAP_SERDES4_IO_STAT 0x00001111 0x00001111

mc MAP_SERDES0_IO_CTRL_1 0x10000000 0x10000000

mc MAP_SERDES1_IO_CTRL_1 0x10000000 0x10000000


   /*************** Do here serdes mux */
