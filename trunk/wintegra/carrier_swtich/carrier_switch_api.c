/*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2013.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include "carrier_switch_util.h"



WP_iw_agg_generic dl_tx_agg_gbe[1] =
{
    {
        /*tag*/ 2,
        /*txfunc*/ 0,
        /*iw_port*/0,
        /*rfcs*/WP_IW_RFCS_ENABLE,
        /*interruptqueue;*/WP_IW_IRQT1,
        /*error_pkt_mode*/WP_IW_ERRPKT_DISCARD,
        /*intmode;*/WP_IW_INT_DISABLE,
        /*statmode;*/WP_IW_STAT_ENABLE,
        /*timestamp_mode;*/WP_IW_TIME_STAMP_DISABLE,
        /*mtu;*/MAX_MTU,
        /*flow_agg_type;*/WP_IW_FLOW_AGG_PRIMARY,
        /*policer_handle;*/0,
        /*pecs_handle;*/0,
        /*pecs_flow_info;*/0,
        /*pecs_global_info_handle;*/0,
    },
};

WPE_vlan_edit_options vlan_edit_options[] =
{
    /* default , VLAN_EDIT_DEFAULT*/
    {
        /* egress_rule[WPE_GPE_BROUTER_PECS_NUM_OF_VLAN_INPUT_TYPES]; */
        {
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_UNTAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
        },
        /* replace_int_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* int_vlan_tag; */               0,
        /* int_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* replace_ext_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* ext_vlan_tag; */               0,
        /* ext_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* vlan_stag_etype; */            0,
        /* replace_sub_port_vlan_mode; */ WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE,
        /* sub_port_vlan_tag; */          0,
    },

    /* add subport vlan, VLAN_EDIT_ADD_SUBVLAN*/
    {
        /* egress_rule[WPE_GPE_BROUTER_PECS_NUM_OF_VLAN_INPUT_TYPES]; */
        {
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_UNTAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
        },
        /* replace_int_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* int_vlan_tag; */               0,
        /* int_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* replace_ext_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* ext_vlan_tag; */               0,
        /* ext_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* vlan_stag_etype; */            0,
        /* replace_sub_port_vlan_mode; */ WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD,
        /* sub_port_vlan_tag; */          0,
    },
    
    /* del subport vlan*/
    {
        /* egress_rule[WPE_GPE_BROUTER_PECS_NUM_OF_VLAN_INPUT_TYPES]; */
        {
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_UNTAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_TAGGED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
            WPE_GPE_BROUTER_PECS_VLAN_EGRESS_RULE_STACKED,
        },
        /* replace_int_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* int_vlan_tag; */               0,
        /* int_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* replace_ext_vlan_mode; */      WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE,
        /* ext_vlan_tag; */               0,
        /* ext_vlan_etype_source; */      WPE_GPE_BROUTER_PECS_VLAN_ETYPE_UNCHANGED,
        /* vlan_stag_etype; */            0,
        /* replace_sub_port_vlan_mode; */ WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_REMOVE,
        /* sub_port_vlan_tag; */          0,
    },

};



WPE_gpe_brouter_pecs_flow_info brouter_pecs_flow_info[] = 
{
    /* default index PECS_FLOW_DEFAULT*/
    {
        WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
        0, /* extraction_size */
        WPE_GPE_BROUTER_PECS_ADD_DISABLE, /* prefix_add_mode */
        0, /* prefix_add_size */
        0, /* prefix_ip_offset */
        WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE, /* mac_replace_mode */
        {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
        {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
        0, /* vlan_edit_options*/   

        {0xff, 0xff, 0xff, 0xff}, /* prefix_remark_options*/
        WPE_GPE_BROUTER_PECS_TTL_DISABLE, /* ttl_mode */
        WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
        {/* prefix */
                0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
                0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
                0x81, 0, 0, 1, 
                0x88,0x47,
                0x11,0x11, 0x10, 0x80,
                0x22,0x22, 0x21, 0x80,
                0,0,0,0,0,0,0,0, 
                0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0, 
                0,0,0,0,0,0,0,0,
                0,0,0,0,0,0,0,0
        }  
    },

    /* UNI index PECS_FLOW_UNI*/
    {
        WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE, /* header_extract_mode */
        0, /* extraction_size */
        WPE_GPE_BROUTER_PECS_ADD_L2_HEADER, /* prefix_add_mode */
        12, /* prefix_add_size */
        0, /* prefix_ip_offset */
        WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE, /* mac_replace_mode */
        {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
        {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
        0, /* vlan_edit_options*/


        {0xff, 0xff, 0xff, 0xff}, /* prefix_remark_options*/
        WPE_GPE_BROUTER_PECS_TTL_CHECK_AND_DECREMENT, /* ttl_mode */
        WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
        {/* prefix */
            0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
            0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
            0x81, 0, 0, 1, 
            0x88,0x47,
            0x11,0x11, 0x10, 0x80,
            0x22,0x22, 0x21, 0x80,
            0,0,0,0,0,0,0,0, 
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0, 
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0
        }  
    },

    /* NNI index PECS_FLOW_NNI*/
    {
        WPE_GPE_BROUTER_PECS_EXTRACT_L2_HEADER, /* header_extract_mode */
        0, /* extraction_size */
        WPE_GPE_BROUTER_PECS_ADD_L2_HEADER, /* prefix_add_mode */
        12, /* prefix_add_size */
        0, /* prefix_ip_offset */
        WPE_GPE_BROUTER_PECS_REPLACE_MAC_DISABLE, /* mac_replace_mode */
        {0x11, 0x22, 0x33, 0x44, 0x55, 0x00}, /* mac_da */
        {0x66, 0x77, 0x88, 0x99, 0xaa, 0x00}, /* mac_sa */
        0, /* vlan_edit_options*/


        {0xff, 0xff, 0xff, 0xff}, /* prefix_remark_options*/
        WPE_GPE_BROUTER_PECS_TTL_CHECK_AND_DECREMENT, /* ttl_mode */
        WPE_GPE_BROUTER_PECS_TOS_REMARKING_DISABLE, /* tos_remarking_mode */
        {/* prefix */
            0x11, 0x22, 0x33, 0x44, 0x55, 0x00,
            0x11, 0x12, 0x13, 0x14, 0x17, 0x00,
            0x81, 0, 0, 1, 
            0x88,0x47,
            0x11,0x11, 0x10, 0x80,
            0x22,0x22, 0x21, 0x80,
            0,0,0,0,0,0,0,0, 
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0, 
            0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0
        }  
    },

};


static WP_dfc_sys_info dfc_sys_info[1] =
   {
      {
         /* classification_hash_size */ WP_IW_512_HASH_ENTRIES,
         /* port_filtering */ 0,
         /* deny_eq_ip */ WP_IW_DENY_EQUAL_IP_DISABLE
      }
   };
   
static WP_iw_bridging_lpm_info lpm_info[] = 
{
    {
        /*WP_handle lpm_default_flow_agg;*/ 0,
        /*WP_U8  default_process_mode;*/    WP_FLOW_ROUTE_PROCESS_PASS,
    },
};

static WP_iw_sys_bridging iw_sys_one_bridging[]=
{
    {
        /* tag */                   0,
        /* max_flows; */            MAX_FLOW_AGG,
        /* classification_mode */   WP_IW_NO_CLASSIFIER,
        /* classifier_config */     {0,0,0,NULL},
        /* learning_mode;*/         WP_IW_BRIDGE_LEARNING_DISABLE,
        /* learning_queue */
        {
            /*int_queue_num*/        WP_IW_IRQT1,
            /*learning_queue_size*/  LEARNING_QUEUE_SIZE,
            /*interrupt_enbale*/     WP_LEARNING_INTENABLE,
            /*interrupt_rate*/       1
        },
        /*forwarding_table_size*/   WP_IW_HASH_UNUSED,
        /*member_set_size*/         WP_IW_HASH_UNUSED,
        /*buffer_gap;*/             0x40,
        /*max_bridging_ports*/      IW_SYS_MAX_BPORTS,
        /*dfc_info:*/               dfc_sys_info,
        /*svl_mode*/                WP_IW_SVL_DISABLED, 
        /*stag_ether_type*/         0/*DEFAULT VALUE*/,
    }
};  


WP_ch_enet enet_ch_config =
{
    /* intmode */         WP_PKTCH_INT_DISABLE,
    /* iwmode */          WP_PKTCH_IWM_ENABLE,
    /* testmode */        WP_PKTCH_TEST_DISABLE,
    /* tx_pqblock */      0,
    /* tx_pqlevel */      0,
    /* tx_shaping_type*/  WP_FMU_SHAPING_TYPE_STRICT,
    /* tx_shaping_params*/NULL,
    /* rx_maxsdu */       WT_MAX_FRAME_SIZE,
    /* tx_cwid */         WP_CW_ID_A,
    /* tx_tq */           0,
    /* rx_queuedepth */   50,
};

WP_iw_mc_member bc_member_config[1] =
{
    {
        /* txfunc */0,
        /* service_class_index */0,
        /* buffers_threshold */0,
        /* iw_port */0,
        /* edit_size */0x20,
        /* data_copy_mode */WP_IW_MC_MEMBER_DATACOPY_DISABLE,
        /*iw_port_check_mode*/WP_IW_MC_MEMBER_IWPORT_CHECK_ENABLE
    }
};




WP_iw_mc_group bc_group_config[1] =
{
    {
        /* type */WP_IW_MC_UNRESTRICTED,
        /* mode */WP_IW_MC_MODE_MEMBERS_STAT_ENABLE | WP_IW_MC_MODE_MEMBERS_ENHANCED_STAT_ENABLE | WP_IW_MC_MODE_GENERIC_GROUP,
        /* max_iw_mc_members */MAX_PORT_PER_SYSTEM,
        /* qnode */0,
        /* encaps_gap */64,
        /* classes_config */NULL,
        /* identifier */WP_UNUSED
    }
};


WP_iw_agg_multicast bc_agg_config[1] =
{
    {
            /* tag */0,
            /* type */WP_IW_FLOW_AGG_PRIMARY,
            /* mc_group */0,
            /* mtu */1536,
            /* intmode */WP_IW_INT_DISABLE,
            /* interruptqueue */WP_IW_IRQT1,
            /* statmode */WP_IW_STAT_ENABLE,
            /* timestamp_mode */WP_IW_TIME_STAMP_DISABLE,
            /* ov_pool_mode */WP_IW_OV_POOL_ENABLE,
            /* fbp_drop_threshold */0,
            /* policer_enable */WP_IW_POLICER_DISABLE,
            /* *policer_config */NULL
    }
};

WP_bridge_port mc_dummy_bport_config[1] =
{
        {
                /* tag */ 0xFFFF,
                /* direct_mapping */ WP_IW_DIRECT_MAP_ENABLE,
                /* flow_agg */ 0,
                /* flooding_term_mode */ WP_IW_HOST_TERM_MODE,
                /* learning_mode */ WP_IW_LEARNING_DISABLE,
                /* in_filter_mode */ WP_IW_INGRESS_FILTER_DISABLE,
                /* vlan_param */
                {
                        /* vlan_acceptance_mode */ WP_IW_ACCEPT_TAGGED_UNTAGGED,
                        /* vlan_tag */ 0,
                },
                /* max_mac_addresses */ 50,
                /* group_tag */ 0,
                /* group_filtering_mode */ WP_IW_GROUP_FILTER_DISABLE,
                /* unk_mac_sa_filter*/ 0,
                /* unk_mc_mode*/ WP_IW_UNK_MC_HT,
                /* bc_ht_mode*/ WP_IW_BC_HT_DISABLE,
                /* input_filters_mask */ 0,
                /* output_filters_mask */ 0,
                /* statmode */ WP_IW_PORT_STAT_ENABLE
        }
};

WP_rx_binding_bridging  rx_binding_cfg[1]=
{
    {
        /*  encap_mode */0,
        /*  mru;*/MAX_MTU,
        /*  vcfcs;*/0,
        /*  input_port;*/0
    }
};

WP_fmu_shaping_wfq fmu_shaping_wfq[] = 
{
    {
        /* weight */ 0,
        /* flags */  0
    }
};

WP_tx_binding tx_binding[1] =
{
   {
     /* Reserved     */            0,
      /* dci_mode */             WP_IW_DYN_CH_INSERT_ENABLE,
      /* maxt */                 1000
   }
};

static const WP_U8 UNI_Group_queue[8] = {0, 1, 2, 0, 1, 2, 3, 0};
//#ifdef WP_HW_WINPATH4
#ifdef _HW_FIFO_NUM_4_
static const WP_U8 UNI_Qos_mapping[8] = {8, 7, 6, 5, 4, 2, 1, 0};
#else
static const WP_U8 UNI_Qos_mapping[8] = {16, 11, 10, 9, 8, 2, 1, 0};
#endif

char* WPE_QosTypeString(WP_U8 mode)
{
    switch (mode)
    {
        case AF_MODE_SP:      return "sp "; break;
        case AF_MODE_WFQ:     return "wfq"; break;
        default:    return "???"; break;    
    }    
}

char* WPE_PortTypeString(WP_U8 type)
{
    switch (type)
    {
        case PORT_TYPE_UNI:     return "UNI"; break;
        case PORT_TYPE_NNI:     return "NNI"; break;
        default:    return "???"; break;    
    }    
}



int WPE_CreatePortQos(WP_U16 port, WP_U16 subport, WP_handle device, WP_handle qnode)
{
    WP_status status;
    WP_U16 ii;

    WT_TP_PORT *pTpPort = NULL;
    WT_SUBPORT *pSubPort = NULL;
    WT_QOS  *pQos = NULL;


    WP_shaping_group_type_packet l1_group_config[] =
    {
       {
             /* group_level */     WP_L1_GROUP,
             /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
             /* shaping_params */  NULL,
             /* num_fifos */       WP_UNUSED,
             /* block_handle */    0,
             /* block_level */     0,
             /* group_mode */      WP_UNUSED
       }
    };
    
    WP_shaping_group_type_packet l2_group_config[] =
    {
       {
            /* group_level */     WP_L2_GROUP,
            /* tx_shaping_type */ WP_FMU_SHAPING_TYPE_STRICT,
            /* shaping_params */  NULL,
            /* num_fifos */       WP_NUM_FIFOS_DEFAULT,
            /* block_handle */    0,
            /* block_level */     0,
            /* group_mode */      WP_MODE_HW
       }
    };

    if ((port < 1) || (port > NR_GBE) || (subport > MAX_SUBPORT_PER_PORT))
    {
        return -1;
    }

    pTpPort = hw_port + port - 1;

    if (0 == subport)
    {
        pQos = &(pTpPort->qos);
    }
    else
    {
        pSubPort = pTpPort->subport + subport - 1;
        pQos = &(pSubPort->qos);
    }
      
      /* shaping groups 
      L1 group per each logical sub-port over the ENET device.
      For each L1 group create a L2 shaping block with 3 L2 groups each. 
      For each L2 shaping block create 3 L2 groups (in strict priority).
      */
      if (0 == pTpPort->l1_blocks[0])
      {
          pTpPort->l1_blocks[0] = WP_ShapingBlockCreate(WP_SYSHANDLE(DEFAULT_WPID),
                                                                      WP_L1_GROUP,
                                                                      NUM_L1_GROUPS,
                                                                      WP_UNUSED);
          App_TerminateOnError(pTpPort->l1_blocks[0], "WP_ShapingBlockCreate() l1 blocks",__LINE__);
      }
      
      if (0 == pQos->fmu.l1_groups[0])
      {
          l1_group_config->block_handle = pTpPort->l1_blocks[0];
          l1_group_config->block_level = subport;
          pQos->fmu.l1_groups[0] = WP_ShapingGroupCreate(device, WP_SHAPING_GROUP_TYPE_ENET, l1_group_config);
          App_TerminateOnError(pQos->fmu.l1_groups[0], "WP_ShapingGroupCreate() l1 groups",__LINE__);
          
          status = WP_ShapingGroupEnable(pQos->fmu.l1_groups[0]);
          App_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);
          
      }
      
      if (0 == pQos->fmu.l2_blocks[0])
      {
          pQos->fmu.l2_blocks[0] = WP_ShapingBlockCreate(WP_SYSHANDLE(DEFAULT_WPID),
                                                                      WP_L2_GROUP,
                                                                      NUM_L2_GROUPS,
                                                                      WP_NUM_FIFOS_DEFAULT);
          App_TerminateOnError(pQos->fmu.l2_blocks[0], "WP_ShapingBlockCreate() l2 blocks",__LINE__);
      }
      
      for (ii = 0; ii < NUM_L2_GROUPS; ii++)
      {
          l2_group_config->block_level = ii;
          l2_group_config->block_handle = pQos->fmu.l2_blocks[0];
          if (0 == pQos->fmu.l2_groups[ii])
          {
              pQos->fmu.l2_groups[ii] = WP_ShapingGroupCreate(pQos->fmu.l1_groups[0], WP_SHAPING_GROUP_TYPE_ENET, l2_group_config);
              App_TerminateOnError(pQos->fmu.l2_groups[ii], "WP_ShapingGroupCreate() l2 groups",__LINE__);
              
              status = WP_ShapingGroupEnable(pQos->fmu.l2_groups[ii]);
              App_TerminateOnError(status, "WP_ShapingGroupEnable", __LINE__);        
          }
      }

    
    // tx channel
    /*       
    For the 1st L2 group 
    -  create 3 HW ENET channels (L3 FIFOs) in SP [CS6, CS7, EF] 
    (note that the remaining 5 L3 FIFOs of this L2 groups are not used)
    */
    if (0 == pQos->fmu.tx_chan_enet[0])
    {
        enet_ch_config.tx_tq = UNI_Group_queue[0];
        enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
        enet_ch_config.tx_shaping_params = NULL;
        pQos->fmu.tx_chan_enet[0] = WP_ChannelCreate(0, pQos->fmu.l2_groups[0], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[0], "WP_Channel_Create() Fast TX", __LINE__);
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[0], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);

        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[0]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
    }

    if (0 == pQos->fmu.tx_chan_enet[1])
    {
        enet_ch_config.tx_tq = UNI_Group_queue[1];
        pQos->fmu.tx_chan_enet[1] = WP_ChannelCreate(0, pQos->fmu.l2_groups[0], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[1], "WP_Channel_Create() Fast TX", __LINE__);
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[1], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[1]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);

    }

    if (0 == pQos->fmu.tx_chan_enet[2])
    {
        enet_ch_config.tx_tq = UNI_Group_queue[2];
        pQos->fmu.tx_chan_enet[2] = WP_ChannelCreate(0, pQos->fmu.l2_groups[0], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[2], "WP_Channel_Create() Fast TX", __LINE__);        
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[2], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[2]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
    }
    /*        
    For the 2nd L2 group 
    ¨C create 4 HW ENET channels (L3 FIFOs) in SP or WFQ [AF1- AF4] 
    (note that the remaining 4 L3 FIFOs of this L2 groups are not used)
    */

    if (0 == pQos->fmu.tx_chan_enet[3])
    {
        if (AF_MODE_WFQ == pQos->af.mode) // wfq mode
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            fmu_shaping_wfq[0].weight = pQos->af.weight[0];
            enet_ch_config.tx_shaping_params = fmu_shaping_wfq;
        }
        else
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
            enet_ch_config.tx_shaping_params = NULL;
        }

        enet_ch_config.tx_tq = UNI_Group_queue[3];
        pQos->fmu.tx_chan_enet[3] = WP_ChannelCreate(0, pQos->fmu.l2_groups[1], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[3], "WP_Channel_Create() Fast TX", __LINE__);
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[3], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[3]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
    }

    if (0 == pQos->fmu.tx_chan_enet[4])
    {
        if (AF_MODE_WFQ == pQos->af.mode) // wfq mode
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            fmu_shaping_wfq[0].weight = pQos->af.weight[1];
            enet_ch_config.tx_shaping_params = fmu_shaping_wfq;
        }
        else
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
            enet_ch_config.tx_shaping_params = NULL;
        }
        
        enet_ch_config.tx_tq = UNI_Group_queue[4];
        pQos->fmu.tx_chan_enet[4] = WP_ChannelCreate(0, pQos->fmu.l2_groups[1], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[4], "WP_Channel_Create() Fast TX", __LINE__);        
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[4], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[4]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
        
    }

    if (0 == pQos->fmu.tx_chan_enet[5])
    {
        if (AF_MODE_WFQ == pQos->af.mode) // wfq mode
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            fmu_shaping_wfq[0].weight = pQos->af.weight[2];
            enet_ch_config.tx_shaping_params = fmu_shaping_wfq;
        }
        else
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
            enet_ch_config.tx_shaping_params = NULL;
        }

        enet_ch_config.tx_tq = UNI_Group_queue[5];
        pQos->fmu.tx_chan_enet[5] = WP_ChannelCreate(0, pQos->fmu.l2_groups[1], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[5], "WP_Channel_Create() Fast TX", __LINE__);
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[5], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[5]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
    }

    if (0 == pQos->fmu.tx_chan_enet[6])
    {
        if (AF_MODE_WFQ == pQos->af.mode) // wfq mode
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            fmu_shaping_wfq[0].weight = pQos->af.weight[3];
            enet_ch_config.tx_shaping_params = fmu_shaping_wfq;
        }
        else
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
            enet_ch_config.tx_shaping_params = NULL;
        }        
        enet_ch_config.tx_tq = UNI_Group_queue[6];
        pQos->fmu.tx_chan_enet[6] = WP_ChannelCreate(0, pQos->fmu.l2_groups[1], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[6], "WP_Channel_Create() Fast TX", __LINE__);        
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[6], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[6]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
    
    }

    /*
    For the 3rd L2 group 
    ¨C create 1 HW ENET channel (L3 FIFO) [BE] 
    (note that the remaining 7 L3 FIFOs of this L2 groups are not used)        
    */
    if (0 == pQos->fmu.tx_chan_enet[7])
    {
        enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
        enet_ch_config.tx_shaping_params = NULL;        
        enet_ch_config.tx_tq = UNI_Group_queue[7];
        pQos->fmu.tx_chan_enet[7] = WP_ChannelCreate(0, pQos->fmu.l2_groups[2], qnode, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pQos->fmu.tx_chan_enet[7], "WP_Channel_Create() Fast TX", __LINE__);        
        status = WP_IwTxBindingCreate(pQos->fmu.tx_chan_enet[7], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);
        status = WP_ChannelEnable(pQos->fmu.tx_chan_enet[7]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);
        
    }

    return 0;
}



int WPE_SetAfmode(WP_U16 port, WP_U16 subport, WP_handle device, WT_QOS* pNewQos)
{
    WP_handle qnode = 0;
    WP_status status;
    WP_U16 ii;
    WT_TP_PORT* pPort = NULL;
    WT_QOS* pOldQos = NULL;
    
    pPort = hw_port + port - 1;

    if (0 == subport)
    {
        // set port af mode
        if (pPort->num_subport)
        {
            return -1;
        }
        
        pOldQos = &(pPort->qos);
    }
    else
    {
        // set subport af mode
        pOldQos = &(pPort->subport[subport - 1].qos);
    }    
    
    switch(pNewQos->af.mode)
    {
        case AF_MODE_SP:
        {
            if (pNewQos->af.mode == pOldQos->af.mode)
            {
                return 0;
            }
            
            break;
        }
        
        case AF_MODE_WFQ:
        {
            if (0 == memcmp(&pOldQos->af, &pNewQos->af, sizeof(pNewQos->af)))
            {
                return 0;
            }
            break;
        }
            
        default:
            printf("Error, wrong mode %d .\n", pNewQos->af.mode);
            return -2;
    }

    if (AF_MODE_NA == pOldQos->af.mode)
    {
        // first 
        if (PORT_HW_XGI == pPort->hw_type)
        {                
            qnode = iw_hier_qnode[pPort->index-1]; // hier qnode
        }
        else
        if (PORT_HW_SGMII == pPort->hw_type)
        {                    
            qnode = (pPort->index & 1) ? iw_hier_qnode[2] : iw_hier_qnode[3]; // hier qnode                    
        }        
        memcpy(&pOldQos->af, &pNewQos->af, sizeof(pNewQos->af));

        return WPE_CreatePortQos(port, subport, pPort->dev_enet, qnode);
    }    

    // modify
    memcpy(&pOldQos->af, &pNewQos->af, sizeof(pOldQos->af));

    // only need to change 3~6 
    for (ii = 3; ii <= 6; ii++)
    {
        status = WP_ChannelDisable(pOldQos->fmu.tx_chan_enet[ii]);
        App_TerminateOnError(status, "WP_ChannelDisable() Fast TX", __LINE__);
        status = WP_ChannelDelete(pOldQos->fmu.tx_chan_enet[ii]);
        App_TerminateOnError(status, "WP_ChannelDelete() Fast TX", __LINE__);
    }

    for (ii = 3; ii <= 6; ii++)
    {
        if (AF_MODE_WFQ == pOldQos->af.mode) // wfq mode
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_WFQ;
            fmu_shaping_wfq[0].weight = pOldQos->af.weight[ii - 3];
            enet_ch_config.tx_shaping_params = fmu_shaping_wfq;
        }
        else
        {
            enet_ch_config.tx_shaping_type = WP_FMU_SHAPING_TYPE_STRICT;
            enet_ch_config.tx_shaping_params = NULL;
        }
        
        enet_ch_config.tx_tq = UNI_Group_queue[ii];

        // Can't use WP_ChannelModify, because SelectMode must be same in one group which is set when the first channel created.
        pOldQos->fmu.tx_chan_enet[ii] = WP_ChannelCreate(0, pOldQos->fmu.l2_groups[1], qniw, WP_CH_TX, WP_ENET, &enet_ch_config);
        App_TerminateOnError(pOldQos->fmu.tx_chan_enet[ii], "WP_ChannelCreate() Fast TX", __LINE__);

        status = WP_IwTxBindingCreate(pOldQos->fmu.tx_chan_enet[ii], WP_IW_TX_BINDING, tx_binding);
        App_TerminateOnError(status, "WP_IwTxBindingCreate Fast TX", __LINE__);

        status = WP_ChannelEnable(pOldQos->fmu.tx_chan_enet[ii]);
        App_TerminateOnError(status, "WP_ChannelEnable Fast TX", __LINE__);

    }

    return 0;
}

void WPE_PortShowInfo(void)
{
    WP_U16 ii;
    WPE_PortListDisplay();    
    
        printf("id|type| vlan|      sa mac      |      da mac      |sub|vpn| af | weight1,2,3,4\n");
        printf("------------------------------------------------------------------------------\n");
    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (!WPE_HWPortIdValid(ii))
        {
            continue;
        }     
        printf("%2u  %s %4u  %02x:%02x:%02x:%02x:%02x:%02x  %02x:%02x:%02x:%02x:%02x:%02x   %2u  %2u  %s  %u, %u, %u, %u\n", 
            hw_port[ii].index, WPE_PortTypeString(hw_port[ii].type), hw_port[ii].defvlan,
            hw_port[ii].sa_mac[0], hw_port[ii].sa_mac[1], hw_port[ii].sa_mac[2], 
            hw_port[ii].sa_mac[3], hw_port[ii].sa_mac[4], hw_port[ii].sa_mac[5],
            hw_port[ii].da_mac[0], hw_port[ii].da_mac[1], hw_port[ii].da_mac[2], 
            hw_port[ii].da_mac[3], hw_port[ii].da_mac[4], hw_port[ii].da_mac[5],
            hw_port[ii].num_subport, hw_port[ii].num_vpnport, WPE_QosTypeString(hw_port[ii].qos.af.mode),
            hw_port[ii].qos.af.weight[0], hw_port[ii].qos.af.weight[1], 
            hw_port[ii].qos.af.weight[2], hw_port[ii].qos.af.weight[3]);
    }
}


int wpe_PortCreatePriMapping(WT_TP_PORT* pPort)
{   
    WP_U16 ii;

    if ((PORT_TYPE_UNI == pPort->type) && (0 == pPort->rule_intpri[0]))
    {
        WP_pce_rule_classification rule_cfg = {0};
        rule_cfg.enabled = WP_ENABLE;   

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = pPort->bport_enet;

        // INT VLAN            
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F2];

        for (ii = 0; ii < 8; ii++)
        {
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
            rule_cfg.rule_fields[1].value.vlan_tag = (ii << 13) & WP_PCE_FIELD_MASK_VLAN_PRIORITY;
            
            rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
            rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_VALUE;

            rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
            
            rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
            rule_cfg.match_result[0].param.priority_mapping.value = ii;
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

            pPort->rule_intpri[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
            App_TerminateOnError(pPort->rule_intpri[ii], "WP_PceRuleCreate", __LINE__);                                              
        
        }

        // EXT VLAN            
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F3];

        for (ii = 0; ii < 8; ii++)
        {
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
            rule_cfg.rule_fields[1].value.vlan_tag = (ii << 13) & WP_PCE_FIELD_MASK_VLAN_PRIORITY;
            
            rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
            rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_VALUE;

            rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
            
            rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
            rule_cfg.match_result[0].param.priority_mapping.value = ii;
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

            pPort->rule_extpri[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
            App_TerminateOnError(pPort->rule_extpri[ii], "WP_PceRuleCreate", __LINE__);                                              
        
        }           
    }
    else
    if ((PORT_TYPE_NNI == pPort->type) && (0 == pPort->rule_intpri[0]))
    {
        WP_pce_rule_classification rule_cfg = {0};
        rule_cfg.enabled = WP_ENABLE;   

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = pPort->bport_enet;

        // INT VLAN            
        rule_cfg.filter_handle = NNI_PCE_filter[FILTER_F1];

        for (ii = 0; ii < 8; ii++)
        {
            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
            rule_cfg.rule_fields[1].value.vlan_tag = (ii << 13) & WP_PCE_FIELD_MASK_VLAN_PRIORITY;
            
            rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
            
            rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
            rule_cfg.match_result[0].param.priority_mapping.value = UNI_Qos_mapping[ii];
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

            pPort->rule_intpri[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
            App_TerminateOnError(pPort->rule_intpri[ii], "WP_PceRuleCreate", __LINE__);                                              
        
        }       
    }

    return 0;
}

int wpe_PortFirstSetup(WT_TP_PORT* pPort, WP_U16 type, WP_U16 defvlan, WP_U8 s_mac[6], WP_U8 d_mac[6])
{   
    int ret;
    WP_status status;
    WP_handle qnode = 0;
    // first
    pPort->type = type;
    
    // set default af mode
    pPort->qos.af.mode = AF_MODE_SP;
    pPort->qos.af.weight[0] = 0;
    pPort->qos.af.weight[1] = 0;
    pPort->qos.af.weight[2] = 0;
    pPort->qos.af.weight[3] = 0;    
    // then create qos and tx channel

    if (PORT_HW_XGI == pPort->hw_type)
    {                
        qnode = iw_hier_qnode[pPort->index-1]; // hier qnode
    }
    else
    if (PORT_HW_SGMII == pPort->hw_type)
    {                    
        qnode = (pPort->index & 1) ? iw_hier_qnode[2] : iw_hier_qnode[3]; // hier qnode                    
    }    
    ret = WPE_CreatePortQos(pPort->index, 0, pPort->dev_enet, qnode);
    if (ret)
    {

        return -1;
    }

    // bport
    bport_conf.flow_agg = common_system.default_flow_aggregation;
    bport_conf.vlan_param.vlan_tag = defvlan;
    pPort->bport_enet = WP_IwPortCreate(common_system.iw_system, &bport_conf);
    App_TerminateOnError(pPort->bport_enet, "WP_IwPortCreate ",__LINE__);

    rx_binding_cfg[0].input_port = pPort->bport_enet;
    rx_binding_cfg[0].pce_if_handle = 0;
    
    status = WP_IwRxBindingCreate(pPort->rx_channel, common_system.iw_system,
                                  qniw,
                                  &rx_binding_cfg[0]);
    App_TerminateOnError(status, "WP_IwRxBindingCreate Enet",__LINE__);


    dl_tx_agg_gbe->txfunc = pPort->qos.fmu.tx_chan_enet[0];
    dl_tx_agg_gbe->iw_port = pPort->bport_enet;
    dl_tx_agg_gbe->tag = (pPort->bport_enet & 0xffff) | (pPort->index << 22);
    dl_tx_agg_gbe->policer_handle = 0;
    dl_tx_agg_gbe->iw_gpmt_handle = 0;
    dl_tx_agg_gbe->pecs_handle = 0;
    dl_tx_agg_gbe->pecs_flow_info=NULL;
    pPort->agg_enet =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                 WP_IW_GENERIC_MODE,
                                                 &dl_tx_agg_gbe);
    App_TerminateOnError(pPort->agg_enet, "WP_IwFlowAggregationCreate()",__LINE__);

    if (PORT_TYPE_NNI == type)
    {
        WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_nni_pre, WP_FALSE);
        pPort->vsi_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VSI);
        pPort->vrf_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VRF);
        pPort->vpws_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VPWS);
    }
    else
    {
        WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_uni_no_subport, WP_FALSE);
    }
    

    wpe_PortCreatePriMapping(pPort);

    pPort->defvlan = defvlan;
    
    memcpy(pPort->sa_mac, s_mac, 6);
    memcpy(pPort->da_mac, d_mac, 6);

    status = WP_PortEnable(pPort->port_enet, WP_DIRECTION_DUPLEX);
    App_TerminateOnError(status, "WP_PortEnable",__LINE__);
    
    status = WP_DeviceEnable(pPort->dev_enet, WP_DIRECTION_DUPLEX);
    App_TerminateOnError(status, "WP_DeviceEnable", __LINE__);

    status = WP_ChannelEnable(pPort->rx_channel);
    App_TerminateOnError(status, "WP_ChannelEnable",__LINE__);
    
    return 0;

}

int WPE_PortSet(WP_U16 port, WP_U16 type, WP_U16 defvlan, WP_U8 s_mac[6], WP_U8 d_mac[6])
{
    
    WP_U16 ii;
    WP_status status;

    WT_TP_PORT* pPort = NULL;
    WP_command command = 0;

    if (!WPE_HWPortIdValid(port-1) || (0 == defvlan) || (0 == memcmp(s_mac, d_mac, 6)))
    {
        return -1;
    }
    

    type++;

    if ((PORT_TYPE_UNI != type)
        && (PORT_TYPE_NNI != type))
    {
        return -4;
    }

    pPort = hw_port + port - 1;

    if (pPort->num_subport || pPort->num_vpnport)
    {
        printf("Subport or vpnport connected.\n");
        return -2;
    }

    if (PORT_TYPE_NA == pPort->type)
    {
        // first
        return wpe_PortFirstSetup(pPort, type, defvlan, s_mac, d_mac);
    }
    else
    if (type != pPort->type)
    {

        if ((PORT_TYPE_UNI == pPort->type) && (pPort->rule_intpri[0]))
        {
            for (ii = 0; ii < 8; ii++)
            {
                WP_PceRuleDelete(pPort->rule_intpri[ii]);
                App_TerminateOnError(pPort->rule_intpri[ii], "WP_PceRuleDelete", __LINE__);
                pPort->rule_intpri[ii] = 0;
                
                WP_PceRuleDelete(pPort->rule_extpri[ii]);
                App_TerminateOnError(pPort->rule_extpri[ii], "WP_PceRuleDelete", __LINE__);
                pPort->rule_extpri[ii] = 0;
            }
        }
        else
        if ((PORT_TYPE_NNI == pPort->type) && (pPort->rule_intpri[0]))
        {
            for (ii = 0; ii < 8; ii++)
            {
                WP_PceRuleDelete(pPort->rule_intpri[ii]);
                App_TerminateOnError(pPort->rule_intpri[ii], "WP_PceRuleDelete", __LINE__);
                pPort->rule_intpri[ii] = 0;
            }         
        }

        pPort->type = type;

        if (PORT_TYPE_NNI == type)
        {
            WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_nni_pre, WP_FALSE);
            if (0 == pPort->vsi_next_round)
            {
                pPort->vsi_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VSI);
            }
            
            if (0 == pPort->vrf_next_round)
            {
                pPort->vrf_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VRF);
            }
            
            if (0 == pPort->vpws_next_round)
            {
                pPort->vpws_next_round = WPE_CreateNextRoundInterface(SYSTEM_TYPE_VPWS);
            } 

        }
        else
        {
            WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_uni_no_subport, WP_FALSE);
        }   
    }

    if (defvlan != pPort->defvlan)
    {
        command |= WP_IW_BPORT_MOD_VLAN_DEFAULT_TAG;
        bport_conf.vlan_param.vlan_tag = defvlan;
        status = WP_IwPortModify(pPort->bport_enet, command, &bport_conf);
        App_TerminateOnError(status, "WP_IwPortModify()", __LINE__);
        status = WP_IwRxBindingModify(pPort->rx_channel, common_system.iw_system,
                                      qniw,
                                      WP_IW_RX_BIND_B_MOD_BPORT_VLAN,
                                      &rx_binding_cfg[0]);
        App_TerminateOnError(status, "WP_IwRxBindingModify()", __LINE__);                                      
        pPort->defvlan = defvlan;
    }

    wpe_PortCreatePriMapping(pPort);
    
    memcpy(pPort->sa_mac, s_mac, 6);
    memcpy(pPort->da_mac, d_mac, 6);

    return 0;
}

int WPE_PortSetAfmode(WP_U16 port, WP_U16 mode, WP_U16 weight1, WP_U16 weight2, WP_U16 weight3, WP_U16 weight4)
{

    WT_TP_PORT* pPort = NULL;
    WT_QOS qos;

    if ((port < 1) || (port > NR_GBE))
    {
        return -1;
    }
    
    pPort = hw_port + port - 1;
    if (AF_MODE_NA == pPort->type)
    {
        printf("Please set port type first.\n");
        return -1;
    }
    
    if ((pPort->num_subport) || (pPort->num_vpnport))
    {
        printf("Error, subport or vpnport non-zero.\n");
        return -2;
    }

    mode++;

    qos.af.mode = mode;
    if (AF_MODE_WFQ == qos.af.mode)
    {
        qos.af.weight[0] = weight1;
        qos.af.weight[1] = weight2;
        qos.af.weight[2] = weight3;
        qos.af.weight[3] = weight4;
    }
    else
    if (AF_MODE_SP == qos.af.mode)
    {
        qos.af.weight[0] = 0;
        qos.af.weight[1] = 0;
        qos.af.weight[2] = 0;
        qos.af.weight[3] = 0;        
    }
    else
    {
        return -3;
    }

    memcpy(&qos.fmu, &pPort->qos.fmu, sizeof(qos.fmu));

    return WPE_SetAfmode(port, 0, pPort->dev_enet, &qos);
}

int WPE_FindSubPortByVlan(WT_TP_PORT* pPort, WP_U16 vlan)
{
    WP_U16 ii;
    for(ii = 0; ii < MAX_SUBPORT_PER_PORT; ii++)
    {
        if (!pPort->subport[ii].valid)
        {
            continue;
        }
        else
        if (vlan == pPort->subport[ii].vlan)
        {
            return pPort->subport[ii].index;
        }
    }

    return 0;
}

WT_SUBPORT* WPE_FindEmptySubPort(WT_TP_PORT* pPort)
{
    WP_U16 ii;
    for(ii = 0; ii < MAX_SUBPORT_PER_PORT; ii++)
    {
        if (!pPort->subport[ii].valid)
        {
            return pPort->subport + ii;
        }
    }

    return NULL;
}


int WPE_DeleteSubPortQos(WT_TP_PORT* pPort, WT_SUBPORT* pSubPort)
{
    WP_status status;
    WP_U16 ii;

    {
        for (ii = 0; ii < NUM_TX_CHANNELS; ii++)
        {
            status = WP_ChannelDisable(pSubPort->qos.fmu.tx_chan_enet[ii]);
            App_TerminateOnError(status, "WP_ChannelDisable Fast TX", __LINE__);
            
            status = WP_ChannelDelete(pSubPort->qos.fmu.tx_chan_enet[ii]);
            App_TerminateOnError(status, "WP_ChannelDelete Fast TX", __LINE__);

            pSubPort->qos.fmu.tx_chan_enet[ii] = 0;
        }
    }
    
    for (ii = 0; ii < NUM_L2_GROUPS; ii++)
    {
        status = WP_ShapingGroupDisable(pSubPort->qos.fmu.l2_groups[ii]);
        App_TerminateOnError(status, "WP_ShapingGroupDisable() l2 groups",__LINE__);
        
        status = WP_ShapingGroupDelete(pSubPort->qos.fmu.l2_groups[ii]);
        App_TerminateOnError(status, "WP_ShapingGroupDelete() l2 groups",__LINE__);
        pSubPort->qos.fmu.l2_groups[ii] = 0;
    }   

    if (pSubPort->qos.fmu.l2_blocks[0])
    {
        status = WP_ShapingBlockDelete(pSubPort->qos.fmu.l2_blocks[0]);
        App_TerminateOnError(status, "WP_ShapingBlockDelete()",__LINE__);
        pSubPort->qos.fmu.l2_blocks[0] = 0;
    }

    if (pSubPort->qos.fmu.l1_groups[0])
    {    
        status = WP_ShapingGroupDisable(pSubPort->qos.fmu.l1_groups[0]);
        App_TerminateOnError(status, "WP_ShapingGroupDisable() l1 groups",__LINE__);
        
        status = WP_ShapingGroupDelete(pSubPort->qos.fmu.l1_groups[0]);
        App_TerminateOnError(status, "WP_ShapingGroupDelete() l1 groups",__LINE__);
        pSubPort->qos.fmu.l1_groups[0] = 0;   
    }

    memset(&(pSubPort->qos), 0, sizeof(pSubPort->qos));

    return 0;
}


void WPE_SubPortShowInfo(void)
{
    WP_U16 ii, jj;
    WT_SUBPORT* pSubport = NULL;
        printf("port|id|vlan|defvlan|      sa mac      |      da mac      |vpn| af | weight1,2,3,4\n");
        printf("----------------------------------------------------------------------------------\n");
        
    for (ii = 0; ii < NR_GBE; ii++)
    {
        if (!WPE_HWPortIdValid(ii))
        {
            continue;
        }
        
        if (0 == hw_port[ii].num_subport)
        {
            continue;
        }

        pSubport = hw_port[ii].subport;
        for (jj = 0; jj < MAX_SUBPORT_PER_PORT; jj++, pSubport++)
        {
            if (!pSubport->valid)
            {
                continue;
            }
        printf(" %u   %2u  %4u  %4u   %02x:%02x:%02x:%02x:%02x:%02x  %02x:%02x:%02x:%02x:%02x:%02x  %2u  %s  %u, %u, %u, %u\n", 
            hw_port[ii].index, pSubport->index, pSubport->vlan, pSubport->defvlan,
            pSubport->sa_mac[0], pSubport->sa_mac[1], pSubport->sa_mac[2], 
            pSubport->sa_mac[3], pSubport->sa_mac[4], pSubport->sa_mac[5],
            pSubport->da_mac[0], pSubport->da_mac[1], pSubport->da_mac[2], 
            pSubport->da_mac[3], pSubport->da_mac[4], pSubport->da_mac[5],
            pSubport->num_vpnport, WPE_QosTypeString(pSubport->qos.af.mode), pSubport->qos.af.weight[0], 
            pSubport->qos.af.weight[1], pSubport->qos.af.weight[2], pSubport->qos.af.weight[3]);
        }
        printf("\n");
    }
}

void WPE_SubPortCreateBportPCErule(WT_TP_PORT* pPort, WT_SUBPORT* pSubPort, WP_handle filter)
{
    WP_pce_rule_classification rule_cfg = {0};
    rule_cfg.enabled = WP_ENABLE;   

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = pPort->bport_enet;

    rule_cfg.filter_handle = filter;            
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_SF_SPVLAN_TPID;
    rule_cfg.rule_fields[1].value.spvlan_tpids.spvlan_tag = pSubPort->vlan;
    
    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    
    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
    rule_cfg.match_result[0].param.iw_port.iw_port_handle = pSubPort->bport_enet;
    
    rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

    pSubPort->rule_iwport = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                          WP_PCE_RULE_CLASSIFICATION,
                                          &rule_cfg);
    App_TerminateOnError(pSubPort->rule_iwport, "WP_PceRuleCreate", __LINE__);                                          

    return ;
}

void WPE_SubPortCreatePriMapping(WT_SUBPORT* pSubPort)
{
    WP_U16 ii;

    WP_pce_rule_classification rule_cfg = {0};
    rule_cfg.enabled = WP_ENABLE;   

    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = pSubPort->bport_enet;

    // INT VLAN            
    rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F2];

    for (ii = 0; ii < 8; ii++)
    {
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = (ii << 13) & WP_PCE_FIELD_MASK_VLAN_PRIORITY;
        
        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_VALUE;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
        
        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
        rule_cfg.match_result[0].param.priority_mapping.value = ii;
        
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

        pSubPort->rule_intpri[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                              WP_PCE_RULE_CLASSIFICATION,
                                              &rule_cfg);
        App_TerminateOnError(pSubPort->rule_intpri[ii], "WP_PceRuleCreate", __LINE__);                                              
        
    }

    // EXT VLAN            
    rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F3];

    for (ii = 0; ii < 8; ii++)
    {
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = (ii << 13) & WP_PCE_FIELD_MASK_VLAN_PRIORITY;
        
        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_VALUE;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
        
        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_PRIORITY_MAPPING;
        rule_cfg.match_result[0].param.priority_mapping.value = ii;
        
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            

        pSubPort->rule_extpri[ii] = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                              WP_PCE_RULE_CLASSIFICATION,
                                              &rule_cfg);
        App_TerminateOnError(pSubPort->rule_extpri[ii], "WP_PceRuleCreate", __LINE__);                                              
        
    }                                          

    return;
}


// return subport index > 0.
int WPE_SubPortCreate(WP_U16 portindex, WP_U16 vlan, WP_U16 defvlan, WP_U8 s_mac[6], WP_U8 d_mac[6])
{
    int ret = 0;
    WP_handle qnode = 0;

    WP_U16 subport_index = 0;

    WT_TP_PORT* pPort = NULL;
    WT_SUBPORT* pSubPort = NULL;

    if (!WPE_HWPortIdValid(portindex-1) || (0 == defvlan) || (0 == memcmp(s_mac, d_mac, 6)))
    {
        return -1;
    }

    pPort = hw_port + portindex - 1;
    if ((PORT_TYPE_UNI != pPort->type) || (pPort->num_vpnport))
    {
        printf("Error, port %u type is %s, vpnport %d.\n", portindex, WPE_PortTypeString(pPort->type), pPort->num_vpnport);
        return -1;
    }
    
    subport_index = WPE_FindSubPortByVlan(pPort, vlan);

    if (subport_index)
    {
        return -1;
    }

    if (pPort->num_subport >= MAX_SUBPORT_PER_PORT)
    {
        return -2;
    }

    pSubPort = WPE_FindEmptySubPort(pPort);
    if (NULL == pSubPort)
    {
        return -2;
    }

    App_PortsCreate(common_system.iw_system, common_system.default_flow_aggregation, &pSubPort->bport_enet, defvlan, pSubPort->index + 100);
    pSubPort->defvlan = defvlan;

    // set default af mode
    pSubPort->qos.af.mode = AF_MODE_SP;
    pSubPort->qos.af.weight[0] = 0;
    pSubPort->qos.af.weight[1] = 0;
    pSubPort->qos.af.weight[2] = 0;
    pSubPort->qos.af.weight[3] = 0;    

    if (PORT_HW_XGI == pPort->hw_type)
    {                
        qnode = iw_hier_qnode[pPort->index-1]; // hier qnode
    }
    else
    if (PORT_HW_SGMII == pPort->hw_type)
    {                    
        qnode = (pPort->index & 1) ? iw_hier_qnode[2] : iw_hier_qnode[3]; // hier qnode                    
    }  

    ret = WPE_CreatePortQos(portindex, pSubPort->index, pPort->dev_enet, qnode);
    if (ret)
    {
        return -3;
    }

    dl_tx_agg_gbe->txfunc = pSubPort->qos.fmu.tx_chan_enet[0];
    dl_tx_agg_gbe->policer_handle = 0;
    dl_tx_agg_gbe->iw_gpmt_handle = 0;
    dl_tx_agg_gbe->iw_port = pSubPort->bport_enet;
    dl_tx_agg_gbe->tag = (pSubPort->bport_enet & 0xffff) | (pSubPort->index << 20);
    dl_tx_agg_gbe->pecs_handle = 0;
    dl_tx_agg_gbe->pecs_flow_info=NULL;
    pSubPort->agg_enet =  WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                 WP_IW_GENERIC_MODE,
                                                 &dl_tx_agg_gbe);
    App_TerminateOnError(pSubPort->agg_enet, "WP_IwFlowAggregationCreate()",__LINE__);

    if (0 == pPort->num_subport)
    {
        WP_U16 ii;
        WP_status status;
        // first subport, change filter set and set subport vlan enable
        WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_uni_subport, WP_TRUE);

        for (ii = 0; ii < 8; ii++)
        {
            if (pPort->rule_intpri[ii])
            {
                status = WP_PceRuleDisable(pPort->rule_intpri[ii]);
                App_TerminateOnError(status, "WP_PceRuleDisable", __LINE__);
            }
            if (pPort->rule_extpri[ii])
            {
                status = WP_PceRuleDisable(pPort->rule_extpri[ii]);
                App_TerminateOnError(status, "WP_PceRuleDisable", __LINE__);
            }
        }
       
    }

    pSubPort->vlan = vlan;
    WPE_SubPortCreateBportPCErule(pPort, pSubPort, UNI_PCE_filter[FILTER_F1]);

    WPE_SubPortCreatePriMapping(pSubPort);

    memcpy(pSubPort->da_mac, d_mac, 6);
    memcpy(pSubPort->sa_mac, s_mac, 6);

    pSubPort->valid = 1;
    pPort->num_subport++;

    return pSubPort->index;
}


int WPE_SubPortDelete(WP_U16 port, WP_U16 subport)
{
    int ret = 0;
    WP_status status;
    
    WP_U16 ii;
    WT_TP_PORT* pPort = NULL;
    WT_SUBPORT* pSubPort = NULL;

    if (!WPE_HWPortIdValid(port-1) || (subport < 1) || (subport > MAX_SUBPORT_PER_PORT))
    {
        return -1;
    }

    pPort = hw_port + port - 1;
    pSubPort = pPort->subport + subport - 1;
    if (0  == pSubPort->valid)
    {
        return -2;
    }

    if (pSubPort->num_vpnport)
    {
        printf("Still %u vpnport connected.\n", pSubPort->num_vpnport);
        return -4;
    }

    status = WP_PceRuleDelete(pSubPort->rule_iwport);
    App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);

    for (ii = 0; ii < 8; ii++)
    {
        status = WP_PceRuleDelete(pSubPort->rule_intpri[ii]);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pSubPort->rule_intpri[ii] = 0;
        
        status = WP_PceRuleDelete(pSubPort->rule_extpri[ii]);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pSubPort->rule_extpri[ii] = 0;
    }

    ret = WPE_DeleteSubPortQos(pPort, pSubPort);
    if (ret)
    {

        return -3;
    }

    status = WP_IwFlowAggregationDelete(pSubPort->agg_enet);
    App_TerminateOnError(status, "WP_IwFlowAggregationDelete", __LINE__);
    pSubPort->agg_enet = 0;

    status = WP_IwPortDelete(pSubPort->bport_enet);
    App_TerminateOnError(status, "WP_IwPortDelete", __LINE__);
    pSubPort->bport_enet = 0;

    memset(pSubPort->sa_mac, 0, 6);
    memset(pSubPort->da_mac, 0, 6);
    pSubPort->vlan = 0;
    pSubPort->defvlan = 0;

    pSubPort->valid = 0;

    pPort->num_subport--;

    if (pPort->num_subport)
    {
        return 0;
    }


    for (ii = 0; ii < 8; ii++)
    {
        if (pPort->rule_intpri[ii])
        {
            status = WP_PceRuleEnable(pPort->rule_intpri[ii]);
            App_TerminateOnError(status, "WP_PceRuleEnable", __LINE__);
        }
        
        if (pPort->rule_extpri[ii])
        {
            status = WP_PceRuleEnable(pPort->rule_extpri[ii]);
            App_TerminateOnError(status, "WP_PceRuleEnable", __LINE__);
        }
    }

    WPE_CreatePceInterface(common_system.iw_system, pPort->rx_channel, filter_set_uni_no_subport, WP_FALSE);

    return 0;
}

int WPE_SubPortSetAfmode(WP_U16 port, WP_U16 subport, WP_U16 mode, WP_U16 weight1, WP_U16 weight2, WP_U16 weight3, WP_U16 weight4)
{

    WT_TP_PORT* pPort = NULL;
    WT_SUBPORT* pSubPort = NULL;
    WT_QOS qos;

    if (!WPE_HWPortIdValid(port-1) || (subport < 1) || (subport > MAX_SUBPORT_PER_PORT))
    {
        return -1;
    }

    pPort = hw_port + port - 1;
    pSubPort = pPort->subport + subport - 1;
    if (!pSubPort->valid)
    {
        printf("Error, subport invalid.\n");
        return -3;
    }
    
    if ((pSubPort->num_vpnport))
    {
        printf("Error, vpnport non-zero.\n");
        return -2;
    }

    mode++;

    qos.af.mode = mode;
    if (AF_MODE_WFQ == qos.af.mode)
    {
        qos.af.weight[0] = weight1;
        qos.af.weight[1] = weight2;
        qos.af.weight[2] = weight3;
        qos.af.weight[3] = weight4;
    }
    else
    if (AF_MODE_SP == qos.af.mode)
    {
        qos.af.weight[0] = 0;
        qos.af.weight[1] = 0;
        qos.af.weight[2] = 0;
        qos.af.weight[3] = 0;        
    }
    else
    {
        return -3;
    }

    memcpy(&qos.fmu, &pSubPort->qos.fmu, sizeof(qos.fmu));

    return WPE_SetAfmode(port, subport, pPort->dev_enet, &qos);    
}

char* WPE_SystemTypeString(WP_U8 type)
{
    switch (type)
    {
        case SYSTEM_TYPE_VSI:       return "VSI "; break;
        case SYSTEM_TYPE_VRF:       return "VRF "; break;
        case SYSTEM_TYPE_VPWS:      return "VPWS"; break;
        default:    return "????"; break;    
    }    
}


void WPE_VPNSystemInfo(void)
{
    WP_U16 ii;
    printf("vpnid | type | gpmt | uni num | nni num | path num\n");
    printf("--------------------------------------------------\n");
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (!vpn_system[ii].valid)
        {
            continue;
        }

    printf(" %3u     %s    %u      %2u        %2u       %2u\n", vpn_system[ii].index, 
    WPE_SystemTypeString(vpn_system[ii].type),
    vpn_system[ii].gpmt_id,
    vpn_system[ii].uni_num,
    vpn_system[ii].nni_num,
    vpn_system[ii].path_num);    
    }
}

WT_VsiSystemInfo* WPE_VPNSystemFindFree(void)
{
    WP_U16 ii;
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (!vpn_system[ii].valid)
        {
            return &vpn_system[ii];
        }
    }

    return NULL;
}

WT_VsiSystemInfo* WPE_GetSystemByHandle(WP_handle handle)
{
    WP_U32 ii;
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (handle == vpn_system[ii].iw_system)
        {
            return (vpn_system + ii);
        }
    }

    return NULL;
}


int wpe_VPNSystemCreateGPMTRule(WT_VsiSystemInfo* pVpn)
{
    WP_pce_rule_forwarding gpmt = {0};
    
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        gpmt.match_action = WP_PCE_RULE_MATCH_CONTINUE;
    }
    else
    {
        gpmt.match_action = WP_PCE_RULE_MATCH_ACCEPT;
    }

    gpmt.enabled = WP_ENABLE;
    gpmt.filter_handle = UNI_PCE_filter[FILTER_F8];
    
    gpmt.rule_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    gpmt.rule_fields[0].value.iw_system_handle = pVpn->iw_system;
    gpmt.rule_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
    
    gpmt.match_result[0].result_type = WP_PCE_RESULT_GENERIC_PRIORITY_MAPPING_TABLE;
    gpmt.match_result[0].param.generic_priority_mapping_table.iw_gpmt_handle = iw_gpmt_table_handle[pVpn->gpmt_id];

    gpmt.match_result[1].result_type = WP_PCE_RESULT_LAST;

    pVpn->pce_gmpt = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_CLASSIFICATION, &gpmt);
    App_TerminateOnError(pVpn->pce_gmpt, "WP_PceRuleCreate", __LINE__);

    return 0;
}


WT_VsiSystemInfo* WPE_VPNSystemCreate(WP_U8 type, WP_U8 gpmt)
{
    WP_status status;
    WP_handle iwsystem = 0;
    WT_VsiSystemInfo* pVpn = WPE_VPNSystemFindFree();
    if (NULL == pVpn)
    {
        printf("No free system.\n");
        return NULL;
    }

    switch (type)
    {
        case SYSTEM_TYPE_VSI:
        {
            iw_sys_one_bridging->lpm_mode = WP_LPM_DISABLED;
            iw_sys_one_bridging->lpm_info = NULL;
            iwsystem = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                 iw_sys_one_bridging);
            App_TerminateOnError(iwsystem, "WP_IwSystemCreate",__LINE__);

            break;
        }
        
        case SYSTEM_TYPE_VRF:
        {
            lpm_info->lpm_default_flow_agg = common_system.default_flow_aggregation;
            iw_sys_one_bridging->lpm_mode = WP_LPM_ENABLED;
            iw_sys_one_bridging->lpm_info = lpm_info;
            iwsystem = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                 iw_sys_one_bridging);
            App_TerminateOnError(iwsystem, "WP_IwSystemCreate",__LINE__);          
            break;
        }
        
        case SYSTEM_TYPE_VPWS:
        {
            iw_sys_one_bridging->lpm_mode = WP_LPM_DISABLED;
            iw_sys_one_bridging->lpm_info = NULL;
        
            iwsystem = WP_IwSystemCreate(WP_WINPATH(DEFAULT_WPID),
                                 WP_IW_VLAN_AWARE_BRIDGING_MODE,
                                 iw_sys_one_bridging);
            App_TerminateOnError(iwsystem, "WP_IwSystemCreate",__LINE__);                                 

            break;
        }
        
        default:
        {
            return NULL;
            break;
        }

    }

    status = WP_IwSystemBuild(iwsystem);
    App_TerminateOnError(status, "WP_IwSystemBuild",__LINE__);

    pVpn->iw_system = iwsystem;

    pVpn->default_flow_aggregation = common_system.default_flow_aggregation;
    pVpn->gpmt_id = type;   // gpmt
    pVpn->type = type;

    wpe_VPNSystemCreateGPMTRule(pVpn);

    pVpn->valid = 1;

    return pVpn;
}

int wpe_VPNSystemCreateFlooding(WT_VsiSystemInfo* pVpn)
{
    WP_pce_rule_classification flooding = {0};
    bc_group_config[0].qnode = qniw_mc;
    
    pVpn->flooding.group_handle = WP_IwMcGroupCreate(WP_WINPATH(DEFAULT_WPID), bc_group_config);
    App_TerminateOnError(pVpn->flooding.group_handle, "WP_IwMcGroupCreate() bc group", __LINE__);
    
    bc_agg_config[0].mc_group = pVpn->flooding.group_handle;
    pVpn->flooding.agg_handle = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                           WP_IW_GENERIC_MC_MODE,
                                                           bc_agg_config);
    App_TerminateOnError(pVpn->flooding.agg_handle, "WP_IwFlowAggregationCreate() bc agg", __LINE__);

    mc_dummy_bport_config[0].flow_agg = pVpn->flooding.agg_handle;
    mc_dummy_bport_config[0].tag = (mc_dummy_bport_config[0].flow_agg & 0xfff) | (pVpn->index << 12);
    pVpn->flooding.dum_bport_handle = WP_IwBridgingPortCreate(pVpn->iw_system, &mc_dummy_bport_config[0]);
    App_TerminateOnError(pVpn->flooding.dum_bport_handle, "WP_IwBridgingPortCreate() dummy bport", __LINE__);

    flooding.enabled = WP_ENABLE;
    flooding.match_action = WP_PCE_RULE_MATCH_CONTINUE;
    
    flooding.rule_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    flooding.rule_fields[0].value.iw_system_handle = pVpn->iw_system;

    flooding.rule_fields[1].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
    flooding.rule_fields[1].value.parser_flags = 0x0; // not MC, so is BC or UC.
    
    flooding.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    flooding.match_result[0].result_type = WP_PCE_RESULT_FLOW_AGG;
    flooding.match_result[0].param.flow_agg.flow_aggregation = pVpn->flooding.agg_handle;

    flooding.match_result[1].result_type = WP_PCE_RESULT_LAST;

    flooding.filter_handle = UNI_PCE_filter[FILTER_F9];
    pVpn->flooding.uni_rule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                          WP_PCE_RULE_CLASSIFICATION,
                                          &flooding);
    App_TerminateOnError(pVpn->flooding.uni_rule, "WP_PceRuleCreate", __LINE__); 
/*
    flooding.filter_handle = NNI_PCE_filter[FILTER_F3];

    pVpn->flooding.nni_rule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                          WP_PCE_RULE_CLASSIFICATION,
                                          &flooding);
    App_TerminateOnError(pVpn->flooding.nni_rule, "WP_PceRuleCreate", __LINE__);      
*/
    return 0;
}



int WPE_VPNSystemCreateVSI(WP_U8 gpmt)
{

    WT_VsiSystemInfo* pVpn;
    
/*  future
    if (gpmt)
    {
        
    }
*/  
    if (system_num[SYSTEM_TYPE_VSI] >= MAX_BRIDGING_SYSTEM)
    {
        printf("Max bridging system %d reached.\n", system_num[SYSTEM_TYPE_VSI]);
        return -3;
    }
    pVpn = WPE_VPNSystemCreate(SYSTEM_TYPE_VSI, gpmt);
    if (NULL == pVpn)
    {
        return -1;
    }

    pVpn->learn_en = 1;
    pVpn->aging_en = 1;
    pVpn->aging_weight = 1;

    wpe_VPNSystemCreateFlooding(pVpn);

    system_num[SYSTEM_TYPE_VSI]++;
    
    return pVpn->index;
}

int WPE_VPNSystemCreateVRF(WP_U8 gpmt)
{
    WT_VsiSystemInfo* pVpn ;
    /*  future
        if (gpmt)
        {
            
        }
    */
    if (system_num[SYSTEM_TYPE_VRF] >= MAX_ROUTING_SYSTEM)
    {
        printf("Max routing system %d reached.\n", system_num[SYSTEM_TYPE_VRF]);
        return -3;
    }    
    pVpn = WPE_VPNSystemCreate(SYSTEM_TYPE_VRF, gpmt);
    if (NULL == pVpn)
    {
        return -1;
    }

    system_num[SYSTEM_TYPE_VRF]++;

    return pVpn->index;
}

int WPE_VPNSystemCreateVPWS(WP_U8 gpmt)
{
    WT_VsiSystemInfo* pVpn;
    /*  future
        if (gpmt)
        {
            
        }
    */  
    if (system_num[SYSTEM_TYPE_VPWS] >= MAX_MPLS_SYSTEM)
    {
        printf("Max VPWS system %d reached.\n", system_num[SYSTEM_TYPE_VPWS]);
        return -3;
    }

    pVpn = WPE_VPNSystemCreate(SYSTEM_TYPE_VPWS, gpmt);
    if (NULL == pVpn)
    {
        return -1;
    }

    system_num[SYSTEM_TYPE_VPWS]++;
    
    return pVpn->index;
}

int WPE_VPNSystemDelete(WP_U8 vpnid)
{
    WP_status status;
    WT_VsiSystemInfo* pVpn;
    if ((vpnid < 1 ) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }

    pVpn = vpn_system + vpnid - 1;

    if ((!pVpn->valid) || (pVpn->uni_num) || (pVpn->nni_num) || (pVpn->path_num))
    {
        return -2;
    }

    pVpn->learn_en = 0;
    // delete fdb
    

    if (pVpn->pce_gmpt)
    {
        status = WP_PceRuleDelete(pVpn->pce_gmpt);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pVpn->pce_gmpt = 0;        
    }

    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        if (pVpn->flooding.count)
        {
            return -3;
        }

        if (pVpn->flooding.uni_rule)
        {
            status = WP_PceRuleDelete(pVpn->flooding.uni_rule);
            App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
            pVpn->flooding.uni_rule = 0;
        }
        if (pVpn->flooding.nni_rule)
        {
            status = WP_PceRuleDelete(pVpn->flooding.nni_rule);
            App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
            pVpn->flooding.nni_rule = 0;
        }

        if (pVpn->flooding.dum_bport_handle)
        {
            status = WP_IwBridgingPortDelete(pVpn->iw_system, pVpn->flooding.dum_bport_handle);
            App_TerminateOnError(status, "WP_IwBridgingPortDelete", __LINE__);
            pVpn->flooding.dum_bport_handle = 0;
        }

        if (pVpn->flooding.agg_handle)
        {
            status = WP_IwFlowAggregationDelete(pVpn->flooding.agg_handle);
            App_TerminateOnError(status, "WP_IwFlowAggregationDelete", __LINE__);
            pVpn->flooding.agg_handle = 0;
        }

        if (pVpn->flooding.group_handle)
        {
            status = WP_IwMcGroupDelete(pVpn->flooding.group_handle);
            App_TerminateOnError(status, "WP_IwMcGroupDelete", __LINE__);
            pVpn->flooding.group_handle = 0;
        }
    }

    status = WP_IwSystemDelete(pVpn->iw_system);
    App_TerminateOnError(status, "WP_IwSystemDelete",__LINE__);

    system_num[pVpn->type]--;
    
    pVpn->iw_system = 0;
    pVpn->valid = 0;
    pVpn->type = SYSTEM_TYPE_NA;

    return 0;
}


void WPE_VPNPortShow(void)
{
    WP_U16 ii, jj;
    WPE_TP_VPNPORT *temp = vpn_port;
        printf("UNI VPN ports list:\n");
        printf(" id  | port-sub | vpnid | s-tag | c-tag | egress stag | egress ctag\n");
        printf("-------------------------------------------------------------------\n");

    for (ii = 0, jj = 0; ii < NUM_OF_VPN_PORT; ii++, temp++)
    {
        if ((temp->valid) && (PORT_TYPE_UNI == temp->type))
        {
        printf("  %3u   %u - %2u       %u    %4u    %4u     %4u         %4u\n", 
        temp->index, temp->portindex, temp->subport, temp->vpnid, 
        temp->param.uni.s_tag, temp->param.uni.c_tag, temp->param.uni.egress_stag, temp->param.uni.egress_ctag);
            if ((++jj % 4) == 0)
            {
                printf("\n");
            }
        }
    }

        temp = vpn_port;
        
        printf("\n");
        printf("NNI VPN ports list:\n");
        printf(" id  | port | vpnid | pw label | lsp label\n");
        printf("------------------------------------------\n");
    for (ii = 0, jj = 0; ii < NUM_OF_VPN_PORT; ii++, temp++)
    {
        if ((temp->valid) && (PORT_TYPE_NNI == temp->type))
        {
        printf(" %3u    %u      %u    %8u    %8u\n", 
        temp->index, temp->portindex, temp->vpnid, 
        temp->param.nni.pw_label, temp->param.nni.lsp_label);
            if ((++jj % 4) == 0)
            {
                printf("\n");
            }        
        }
    }

}

WPE_TP_VPNPORT* WPE_VPNPortGetFree(void)
{
    WP_U16 ii;
    for (ii = 0; ii < NUM_OF_VPN_PORT; ii++)
    {
        if (!vpn_port[ii].valid)
        {
            return (vpn_port + ii);
        }
    }

    return NULL;
}

int wpe_VPNPortCreateLearningRule(WT_TP_PORT* pPort, WT_SUBPORT* pSubPort, WPE_TP_VPNPORT* pVpnPort, TP_UNI tag, WP_boolean vsi)
{
    WP_pce_rule_classification rule_cfg = {0};

    if ((NULL == pVpnPort) || (NULL == pPort))
    {
        printf("Invalid input.\r\n");
        return -1;
    }

    rule_cfg.enabled = WP_ENABLE;
    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
    
    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    if (pSubPort)
    {
        rule_cfg.rule_fields[0].value.iw_port_handle = pSubPort->bport_enet;
    }
    else
    {
        rule_cfg.rule_fields[0].value.iw_port_handle = pPort->bport_enet;        
    }

    if (tag.c_tag)
    {        
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F4];

        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = tag.s_tag;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
        rule_cfg.rule_fields[2].value.vlan_tag = tag.c_tag;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[3].value.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_VALUE;

        rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
    }
    else
    if (tag.s_tag)
    {
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F5];

        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag = tag.s_tag;
        
        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_VALUE;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
    }
    else
    {
        return -2;
    }   

    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
    rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpnPort->bport;
   
    if (vsi)
    {
        /* for learning flow agg */
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
        rule_cfg.match_result[1].param.learning_flow_agg.flow_aggregation = pVpnPort->flowagg;                        

        if (pVpnPort->policer)
        {
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[2].param.ingress_policer.policer = pVpnPort->policer->handle;
            
            rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
        }
        else
        {
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
        }        
    }
    else
    {
        if (pVpnPort->policer)
        {
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[1].param.ingress_policer.policer = pVpnPort->policer->handle;
            
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
        }
        else
        {
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
        }           
    }
       
    pVpnPort->learning = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_RULE_CLASSIFICATION,
                                       &rule_cfg);                                          
    App_TerminateOnError(pVpnPort->learning, "WP_PceRuleCreate", __LINE__);

    return 0;
}

// vlan translation, yes/no
WP_boolean wpe_VPNPortUNICreateVlanModRule(WPE_TP_VPNPORT* pVpnPort, TP_UNI tag, WP_U8 systype)
{
    WP_pce_rule_classification rule_cfg = {0};

    if ((0 == tag.egress_stag) && (0 == tag.egress_ctag))
    {
        return WP_FALSE;
    }
    
    rule_cfg.enabled = WP_ENABLE;
    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
    
    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = pVpnPort->bport;

    if (tag.c_tag)
    {
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F6];
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag= tag.s_tag;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
        rule_cfg.rule_fields[2].value.vlan_tag = tag.c_tag;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[3].value.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_VALUE;

        rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;

        if ((tag.egress_stag) && (tag.egress_ctag))
        {
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
            rule_cfg.match_result[0].param.ext_vlan.vlan_tag = tag.egress_stag;
            rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
            rule_cfg.match_result[1].param.int_vlan.vlan_tag = tag.egress_ctag;
            rule_cfg.match_result[1].param.int_vlan.mask = WP_PCE_MASK_VLAN_ID;
            
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
        }
        else
        if ((tag.egress_stag))
        {
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_EXT_VLAN_UPDATE;
            rule_cfg.match_result[0].param.ext_vlan.vlan_tag = tag.egress_stag;
            rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID;
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;            
        }
        else
        if ((tag.egress_ctag))
        {
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
            rule_cfg.match_result[0].param.int_vlan.vlan_tag = tag.egress_ctag;
            rule_cfg.match_result[0].param.int_vlan.mask = WP_PCE_MASK_VLAN_ID;
            
            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;              
        }      

    }
    else
    {
        rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F7];
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
        rule_cfg.rule_fields[1].value.vlan_tag= tag.s_tag;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
        rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_VALUE;

        rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_INT_VLAN_UPDATE;
        rule_cfg.match_result[0].param.ext_vlan.vlan_tag = tag.egress_stag;
        rule_cfg.match_result[0].param.ext_vlan.mask = WP_PCE_MASK_VLAN_ID; 

        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
    }

    pVpnPort->vlanrule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                          WP_PCE_RULE_CLASSIFICATION,
                                          &rule_cfg);
    App_TerminateOnError(pVpnPort->vlanrule, "WP_PceRuleCreate", __LINE__);                

    return WP_TRUE;
}

// create rule for both direction
int wpe_VPNPortCreateVPWSRule(WT_VsiSystemInfo* pVpn)
{
    WP_handle uni_bport;
    WT_TP_PORT* pUNIPort = hw_port + pVpn->pUNI->portindex - 1;
    WT_TP_PORT* pNNIPort = hw_port + pVpn->pNNI->portindex - 1;
    WT_SUBPORT* pSubPort = NULL;
    WP_pce_rule_classification rule_cfg = {0};

    if (pVpn->pUNI->subport)
    {
        pSubPort = pUNIPort->subport + pVpn->pUNI->subport - 1;
        uni_bport = pSubPort->bport_enet;
    }
    else
    {
        uni_bport = pUNIPort->bport_enet;
    }
    
    // nni -> uni
    if (0 == pVpn->pNNI->vpwsrule)
    {    
        memset(&rule_cfg, 0, sizeof(rule_cfg));
        rule_cfg.enabled = WP_ENABLE;
        rule_cfg.filter_handle = NNI_PCE_filter[FILTER_F2];
        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
        
        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = pNNIPort->bport_enet;
        
        rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MPLS_HEADER1;
        rule_cfg.rule_fields[1].value.mpls_label = pVpn->pNNI->param.nni.pw_label << 4;

        rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
        
        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpn->pNNI->bport;
        
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[1].param.flow_agg.flow_aggregation = pVpn->pUNI->flowagg;

        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_PCE_INTERFACE;
        rule_cfg.match_result[2].param.pce_if_iw_next_round.pce_interface = pNNIPort->vpws_next_round;

        rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;               
        
        pVpn->pNNI->vpwsrule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
        App_TerminateOnError(pVpn->pNNI->vpwsrule, "WP_PceRuleCreate", __LINE__);   
    }

    // uni -> nni
    if (0 == pVpn->pUNI->vpwsrule)
    {
        memset(&rule_cfg, 0, sizeof(rule_cfg));
        rule_cfg.enabled = WP_ENABLE;

        rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;    

        rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
        rule_cfg.rule_fields[0].value.iw_port_handle = uni_bport;

        if (pVpn->pUNI->param.uni.c_tag)
        {
            rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F4];

            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
            rule_cfg.rule_fields[1].value.vlan_tag = pVpn->pUNI->param.uni.s_tag;

            rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
            rule_cfg.rule_fields[2].value.vlan_tag = pVpn->pUNI->param.uni.c_tag;

            rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
            rule_cfg.rule_fields[3].value.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_VALUE;

            rule_cfg.rule_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
        }
        else
        {
            rule_cfg.filter_handle = UNI_PCE_filter[FILTER_F5];

            rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
            rule_cfg.rule_fields[1].value.vlan_tag = pVpn->pUNI->param.uni.s_tag;
            
            rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
            rule_cfg.rule_fields[2].value.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_VALUE;

            rule_cfg.rule_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
        }

        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpn->pUNI->bport;

        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule_cfg.match_result[1].param.flow_agg.flow_aggregation = pVpn->pNNI->flowagg;

        if (pVpn->pUNI->policer)
        {
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[2].param.ingress_policer.policer = pVpn->pUNI->policer->handle;
            
            rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;            
        }
        else
        {
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
        } 
        
        pVpn->pUNI->vpwsrule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                                  WP_PCE_RULE_CLASSIFICATION,
                                                  &rule_cfg);
        App_TerminateOnError(pVpn->pUNI->vpwsrule, "WP_PceRuleCreate", __LINE__); 
    }

    return 0;
}

/*
Packet Editing NNI to UNI
L3 VPN
    Remove L2 Eth
    Remove MPLS
    Update TTL
    Add New L2 Eth header
    Add Sub port vlan
    
L2 VPN/VPWS
    Remove L2 Eth
    Remove MPLS
    Update S-Tag(optional for VPLS)
    Add Sub Port Vlan

*/
WP_handle wpe_VPNPortUNICreateFlowAgg(WT_VsiSystemInfo* pVpn, WPE_TP_VPNPORT* pVpnPort, WT_TP_PORT* pPort, WT_SUBPORT* pSubPort)
{
    brouter_pecs_flow_info[PECS_FLOW_UNI].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE;// done by PCE next round interface
    brouter_pecs_flow_info[PECS_FLOW_UNI].extraction_size = 0;

    if (SYSTEM_TYPE_VRF == pVpn->type)
    {
        WP_U16 defvlan = pPort->defvlan;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_L2_HEADER;
        brouter_pecs_flow_info[PECS_FLOW_UNI].ttl_mode = WPE_GPE_BROUTER_PECS_TTL_CHECK_AND_DECREMENT;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_size = L2_HEADER_LENGTH + 6; // 8100 + 0000+ 08 00
        if (pSubPort)
        {
            defvlan = pSubPort->defvlan;
            memcpy(brouter_pecs_flow_info[PECS_FLOW_UNI].prefix, pSubPort->da_mac, 6);
            memcpy(brouter_pecs_flow_info[PECS_FLOW_UNI].prefix + 6, pSubPort->sa_mac, 6);        
        }
        else
        {
            memcpy(brouter_pecs_flow_info[PECS_FLOW_UNI].prefix, pPort->da_mac, 6);
            memcpy(brouter_pecs_flow_info[PECS_FLOW_UNI].prefix + 6, pPort->sa_mac, 6);
        }
        // sub port vlan will be added to the offset 12 of the original header
        // but it is L3 header, so add it manually.
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_DISABLE;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[12] = 0x81;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[13] = 0x00;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[14] = (defvlan >> 8) & 0x0f;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[15] = (defvlan) & 0xff;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[16] = 0x08;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix[17] = 0x00;        

    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_DISABLE;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_size = 0;
        brouter_pecs_flow_info[PECS_FLOW_UNI].ttl_mode = WPE_GPE_BROUTER_PECS_TTL_DISABLE;
        if (pSubPort)
        {
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD;
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].sub_port_vlan_tag = pSubPort->vlan;
        }
        else
        {
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_REMOVE;
        }        
    }
    else
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_DISABLE;
        brouter_pecs_flow_info[PECS_FLOW_UNI].prefix_add_size = 0;
        brouter_pecs_flow_info[PECS_FLOW_UNI].ttl_mode = WPE_GPE_BROUTER_PECS_TTL_DISABLE;

        if (pSubPort)
        {
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_ADD;
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].sub_port_vlan_tag = pSubPort->vlan;
        }
        else
        {
            vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_sub_port_vlan_mode = WPE_GPE_BROUTER_PECS_SUB_PORT_VLAN_REMOVE;
        }        
    }


    if (pVpnPort->param.uni.c_tag)
    {
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID_OVERRIDE_PCE;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].int_vlan_tag = pVpnPort->param.uni.c_tag;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID_OVERRIDE_PCE;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].ext_vlan_tag = pVpnPort->param.uni.s_tag;
    }
    else
    if (pVpnPort->param.uni.s_tag)
    {
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_ID_OVERRIDE_PCE;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].int_vlan_tag = pVpnPort->param.uni.s_tag;
    }
    else
    {
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_int_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;
        vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN].replace_ext_vlan_mode = WPE_GPE_BROUTER_PECS_REPLACE_VLAN_DISABLE;        
    }        


    brouter_pecs_flow_info[PECS_FLOW_UNI].vlan_edit_options = &vlan_edit_options[VLAN_EDIT_ADD_SUBVLAN];
    
    dl_tx_agg_gbe->iw_port = pVpnPort->bport;
    dl_tx_agg_gbe->tag = (pVpnPort->bport & 0xffff) | (pVpnPort->index << 16);
    dl_tx_agg_gbe->iw_gpmt_handle = 0; // only for UNI2NNI
    dl_tx_agg_gbe->policer_handle = 0; // only for UNI2NNI
    
    if (pSubPort)
    {
        dl_tx_agg_gbe->txfunc = pSubPort->qos.fmu.tx_chan_enet[0];
    }
    else
    {
        dl_tx_agg_gbe->txfunc = pPort->qos.fmu.tx_chan_enet[0];
    }
  
    dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
    dl_tx_agg_gbe->pecs_flow_info = (void *)&brouter_pecs_flow_info[PECS_FLOW_UNI];
    pVpnPort->flowagg = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                 WP_IW_GENERIC_MODE,
                                                 dl_tx_agg_gbe);
    App_TerminateOnError(pVpnPort->flowagg, "WP_IwFlowAggregationCreate()",__LINE__);
    
    return pVpnPort->flowagg;

}



/*
Packet Editing UNI to NNI

L3 VPN
    Remove Sub port vlan
    Extract L2 Eth header
    Update TTL
    Add MPLS Header(PW+LSP labels)
    Add new L2 Eth Header
    
L2 VPN/VPWS
    Remove Sub port vlan
    Update S-Tag(optional for VPLS)
    Add MPLS Header
    Add new L2 Eth Header
*/
WP_handle wpe_VPNPortNNICreateFlowAgg(WT_VsiSystemInfo* pVpn, WPE_TP_VPNPORT* pVpnPort, WT_TP_PORT* pPort)
{
    WP_U32  temp;

    

    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_add_mode = WPE_GPE_BROUTER_PECS_ADD_L2_HEADER;

    memcpy(brouter_pecs_flow_info[PECS_FLOW_NNI].prefix, pPort->da_mac, 6);
    memcpy(brouter_pecs_flow_info[PECS_FLOW_NNI].prefix + 6, pPort->sa_mac, 6);
    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[12] = 0x81;
    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[13] = 0x00;

    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[14] = (pPort->defvlan >> 8) & 0x0f;
    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[15] = pPort->defvlan & 0xff;

    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[16] = 0x88;
    brouter_pecs_flow_info[PECS_FLOW_NNI].prefix[17] = 0x47;

    if (SYSTEM_TYPE_VRF == pVpn->type)
    {
        brouter_pecs_flow_info[PECS_FLOW_NNI].vlan_edit_options = NULL;
        brouter_pecs_flow_info[PECS_FLOW_NNI].ttl_mode = WPE_GPE_BROUTER_PECS_TTL_CHECK_AND_DECREMENT;
        brouter_pecs_flow_info[PECS_FLOW_NNI].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_L2_HEADER;  
        brouter_pecs_flow_info[PECS_FLOW_NNI].extraction_size = 0;
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_add_size = L2_MPLS_ADD_SIZE + 4; // + LSP label

        temp = (pVpnPort->param.nni.lsp_label << 12) | DEFAULT_TTL;
        memcpy(brouter_pecs_flow_info[PECS_FLOW_NNI].prefix + 18, &temp, 4);

        temp = (pVpnPort->param.nni.pw_label << 12)  | 0x100 | DEFAULT_TTL;        
        memcpy(brouter_pecs_flow_info[PECS_FLOW_NNI].prefix + 22, &temp, 4);

        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[0] = 28; // remark vlan priority
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[1] = 41; // remark pw exp
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[2] = 49; // remark lsp exp
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[3] = 0xff;
    }
    else
    {
        brouter_pecs_flow_info[PECS_FLOW_NNI].vlan_edit_options = &vlan_edit_options[VLAN_EDIT_DEL_SUBVLAN];    
        brouter_pecs_flow_info[PECS_FLOW_NNI].ttl_mode = WPE_GPE_BROUTER_PECS_TTL_DISABLE;
        brouter_pecs_flow_info[PECS_FLOW_NNI].header_extract_mode = WPE_GPE_BROUTER_PECS_EXTRACT_DISABLE;
        brouter_pecs_flow_info[PECS_FLOW_NNI].extraction_size = 0;
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_add_size = L2_MPLS_ADD_SIZE;

        temp = (pVpnPort->param.nni.pw_label << 12) | 0x100 | DEFAULT_TTL;
        memcpy(brouter_pecs_flow_info[PECS_FLOW_NNI].prefix + 18, &temp, 4);
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[0] = 28; // remark vlan priority
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[1] = 41; // remark pw exp
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[2] = 0xff;
        brouter_pecs_flow_info[PECS_FLOW_NNI].prefix_remark_offset[3] = 0xff;
    }

    dl_tx_agg_gbe->txfunc = pPort->qos.fmu.tx_chan_enet[0];
    dl_tx_agg_gbe->iw_port = pVpnPort->bport;
    dl_tx_agg_gbe->tag = (pVpnPort->bport & 0xffff) | (pVpnPort->index << 16);
    dl_tx_agg_gbe->iw_gpmt_handle = 0;  // updated by PCE result
    
    dl_tx_agg_gbe->pecs_handle = pecs_handles[0];
    dl_tx_agg_gbe->pecs_flow_info = (void *)&brouter_pecs_flow_info[PECS_FLOW_NNI];
    pVpnPort->flowagg = WP_IwFlowAggregationCreate(WP_WINPATH(DEFAULT_WPID),
                                                 WP_IW_GENERIC_MODE,
                                                 &dl_tx_agg_gbe);
    App_TerminateOnError(pVpnPort->flowagg, "WP_IwFlowAggregationCreate()",__LINE__);
    
    return pVpnPort->flowagg;
}


int wpe_VPNPortCreateMPLSRule(WT_VsiSystemInfo* pVpn, WPE_TP_VPNPORT* pVpnPort, WT_TP_PORT* pPort)
{
    WP_pce_rule_classification rule_cfg = {0};

    rule_cfg.enabled = WP_ENABLE;
    rule_cfg.filter_handle = NNI_PCE_filter[FILTER_F2];
    rule_cfg.match_action = WP_PCE_RULE_MATCH_CONTINUE;
    
    rule_cfg.rule_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    rule_cfg.rule_fields[0].value.iw_port_handle = pPort->bport_enet;
    
    rule_cfg.rule_fields[1].field_id = WP_PCE_FIELD_ID_MPLS_HEADER1;
    rule_cfg.rule_fields[1].value.mpls_label = pVpnPort->param.nni.pw_label << 4;

    rule_cfg.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    
    rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
    rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpnPort->bport;
    
    if (SYSTEM_TYPE_VRF == pVpn->type)
    {
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PCE_INTERFACE;    
        rule_cfg.match_result[1].param.pce_if_iw_next_round.pce_interface = pPort->vrf_next_round;
        
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PCE_INTERFACE;    
        rule_cfg.match_result[1].param.pce_if_iw_next_round.pce_interface = pPort->vsi_next_round;
        
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
    }
    else
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_PCE_INTERFACE;
        rule_cfg.match_result[1].param.pce_if_iw_next_round.pce_interface = pPort->vpws_next_round;
        
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
    }
    
    pVpnPort->nextround = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                              WP_PCE_RULE_CLASSIFICATION,
                                              &rule_cfg);
    App_TerminateOnError(pVpnPort->nextround, "WP_PceRuleCreate", __LINE__);
    
    return 0;
}


/*
Specify sub port=0 if it is not used. Configure Zero in c-tag in case the vpn ports associated with a single VID.
Configuring a VPWS connection is achieved by creating UNI and NNI vpn ports using the same VPWS ID(VPN ID that 
represents the relevant VPWS)
When one of the egress tags is defined(not set to 0) then vlan translation will be performed for VSI and VPWS ports. 
Both egress s and c tags must be configured if the vpn port is associated with 2 vlans. Only egress s tag can be 
configured if the port is associated with 1 vlan.
If no vlan translation is required then set the egress tags the same as the ingress tags). 
*/
int WPE_VPNPortCreateUNI(WP_U8 port, WP_U8 subport, WP_U16 vpnid, WP_U16 s_tag, WP_U16 c_tag, WP_U16 egress_s_tag, WP_U16 egress_c_tag)
{
    int ret = 0;
    WP_U16 defvlan = 0;
    TP_UNI tagparam;
    WT_TP_PORT* pPort = NULL;
    WT_SUBPORT* pSubPort = NULL;    
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;

    if (!WPE_HWPortIdValid(port-1) || (subport > MAX_SUBPORT_PER_PORT) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }

    pVpn = vpn_system + vpnid - 1;
    if (!pVpn->valid)
    {
        return -2;
    }

    if ((SYSTEM_TYPE_VPWS == pVpn->type) && (pVpn->uni_num))
    {
        printf("Already one uni in this VPWS.\n");
        return -2;
    }

    if (pVpn->uni_num >= MAX_UNIPORT_PER_SYSTEM)
    {
        printf("Vpn max uni reached.\n");
        return -2;
    }

    pPort = hw_port + port - 1;
    if (PORT_TYPE_UNI != pPort->type)
    {
        printf("Error, port %u type is %s.\n", port, WPE_PortTypeString(pPort->type));
        return -3;
    }

    if ((0 == s_tag))
    {
        printf("Error, wrong stag %u.\n", s_tag);
        return -4;
    }

    defvlan = pPort->defvlan;
    if (subport)
    {
        pSubPort = pPort->subport + subport - 1;
        if (!pSubPort->valid)
        {
            return -4;
        }
        defvlan = pSubPort->defvlan;
    }
    else
    if (pPort->num_subport)
    {
        return -5;
    }

    pVpnPort = WPE_VPNPortGetFree();
    if (NULL == pVpnPort)
    {
        printf("No free VPN port.\n");
        return -6;
    }

    App_PortsCreate(pVpn->iw_system, pVpn->default_flow_aggregation, &pVpnPort->bport, defvlan, pVpnPort->index + 10000);
    
    tagparam.s_tag = s_tag;
    tagparam.c_tag = c_tag;
    tagparam.egress_stag = egress_s_tag;
    tagparam.egress_ctag = egress_c_tag;    
    ret = wpe_VPNPortUNICreateVlanModRule(pVpnPort, tagparam, pVpn->type);

    memcpy(&pVpnPort->param.uni, &tagparam, sizeof(tagparam));
    wpe_VPNPortUNICreateFlowAgg(pVpn, pVpnPort, pPort, pSubPort);

    pVpnPort->vpnid = vpnid;
    pVpnPort->portindex = port;
    pVpnPort->subport   = subport;
    pVpnPort->type = PORT_TYPE_UNI;
    
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        pVpn->pUNI = pVpnPort;
        if (pVpn->pNNI && pVpn->pUNI)
        {
            wpe_VPNPortCreateVPWSRule(pVpn);
        }        
    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {        
        // add it to flooding group
        WP_U32 i = 0;
        for(i = 0; i < MAX_PORT_PER_SYSTEM; i++)//check if vlan flooding group exists
        {
            if(0 == pVpn->flooding.member_handle[i])
            {
                bc_member_config[0].txfunc = pVpnPort->flowagg;//enet tx flow agg
                bc_member_config[0].iw_port = pVpnPort->bport; //output iw port connected to the flow agg 
                pVpn->flooding.member_handle[i] = WP_IwMcMemberAdd(
                        pVpn->flooding.group_handle, bc_member_config);
                App_TerminateOnError(pVpn->flooding.member_handle[i],
                                     "WP_IwMcMemberAdd() bc member", __LINE__);
                pVpn->flooding.count++;
                pVpnPort->bcmember = pVpn->flooding.member_handle[i];

                break;
            }
        }
        
        wpe_VPNPortCreateLearningRule(pPort, pSubPort, pVpnPort, tagparam, WP_TRUE);        
    }
    else
    {
        wpe_VPNPortCreateLearningRule(pPort, pSubPort, pVpnPort, tagparam, WP_FALSE); 
    }
    
    pVpnPort->valid = 1;

    pVpn->uni_num++;

    if (pSubPort)
    {
        pSubPort->num_vpnport++;
    }
    else
    {
        pPort->num_vpnport++;
    }


    return pVpnPort->index;
}

int WPE_VPNPortCreateNNI(WP_U8 port, WP_U16 vpnid, WP_U32 pw_label, WP_U32 lsp_label)
{

    
    WT_TP_PORT* pPort = NULL;
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;

    if (!WPE_HWPortIdValid(port-1) || (vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }

    if ((pw_label & 0xfff00000) || (lsp_label & 0xfff00000))
    {
        printf("Invalid label, pw 0x%x, lsp 0x%x\n", pw_label, lsp_label);
        return -2;
    }

    pVpn = vpn_system + vpnid - 1;
    if (!pVpn->valid)
    {
        return -2;
    }
    
    if ((SYSTEM_TYPE_VPWS == pVpn->type) && (pVpn->nni_num))
    {
        printf("Already one nni in this VPWS.\n");
        return -2;
    }
    
    if (pVpn->nni_num >= MAX_NNIPORT_PER_SYSTEM)
    {
        printf("Vpn max nni reached.\n");
        return -2;
    }

    pPort = hw_port + port - 1;
    if (PORT_TYPE_NNI != pPort->type)
    {
        printf("Error, port %u type is %s.\n", port, WPE_PortTypeString(pPort->type));
        return -3;
    }

    pVpnPort = WPE_VPNPortGetFree();
    if (NULL == pVpnPort)
    {
        printf("No free VPN port.\n");
        return -6;
    }

    App_PortsCreate(pVpn->iw_system, pVpn->default_flow_aggregation, &pVpnPort->bport, 0, pVpnPort->index + 20000);

    pVpnPort->param.nni.pw_label = pw_label;
    pVpnPort->param.nni.lsp_label = lsp_label;

    wpe_VPNPortNNICreateFlowAgg(pVpn, pVpnPort, pPort);

    pVpnPort->valid = 1;
    pVpnPort->vpnid = vpnid;
    pVpnPort->type = PORT_TYPE_NNI;
    pVpnPort->portindex = port;
    pVpnPort->subport   = 0;

    pVpn->nni_num++;
    pPort->num_vpnport++;

    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        pVpn->pNNI = pVpnPort;
        if (pVpn->pNNI && pVpn->pUNI)
        {
            wpe_VPNPortCreateVPWSRule(pVpn);
        }
    }
    else
    {
        wpe_VPNPortCreateMPLSRule(pVpn, pVpnPort, pPort);
        if (SYSTEM_TYPE_VSI == pVpn->type)
        {        
            // add it to flooding group
            WP_U32 i = 0;
            for(i = 0; i < MAX_PORT_PER_SYSTEM; i++)//check if vlan flooding group exists
            {
                if(0 == pVpn->flooding.member_handle[i])
                {
                    bc_member_config[0].txfunc = pVpnPort->flowagg;//enet tx flow agg
                    bc_member_config[0].iw_port = pVpnPort->bport; //output iw port connected to the flow agg 
                    pVpn->flooding.member_handle[i] = WP_IwMcMemberAdd(
                            pVpn->flooding.group_handle, bc_member_config);
                    App_TerminateOnError(pVpn->flooding.member_handle[i],
                                         "WP_IwMcMemberAdd() bc member", __LINE__);
                    pVpn->flooding.count++;
                    pVpnPort->bcmember = pVpn->flooding.member_handle[i];

                    break;
                }
            }

        }                
    }

    return pVpnPort->index;
}


int WPE_VPNPortDelete(WP_U8 vpnport)
{
    WP_U16 i = 0;
    WP_status status;
    WPE_TP_VPNPORT* pVpnPort = NULL;
    WT_VsiSystemInfo* pVpn = NULL;
    WT_TP_PORT* pPort = NULL;
    WT_SUBPORT* pSubPort = NULL;
    if ((vpnport < 1) || (vpnport > NUM_OF_VPN_PORT))
    {
        return -1;
    }
    
    pVpnPort = vpn_port + vpnport - 1;
    if (!pVpnPort->valid)
    {
        return -2;
    }

    if (pVpnPort->policer)
    {
        printf("Policer attached.\n");
        return -3;
    }
    
    if (pVpnPort->item_num)
    {
        printf("Mac table or IP route not empty.\n");
        return -5;

    }

    pVpn = vpn_system + pVpnPort->vpnid - 1;
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        if (pVpn->pNNI && pVpn->pNNI->vpwsrule)
        {
            status = WP_PceRuleDelete(pVpn->pNNI->vpwsrule);
            App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
            pVpn->pNNI->vpwsrule = 0;
        }
        
        if (pVpn->pUNI && pVpn->pUNI->vpwsrule)
        {
            status = WP_PceRuleDelete(pVpn->pUNI->vpwsrule);
            App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
            pVpn->pUNI->vpwsrule = 0;
        }
    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        // remove it from flooding group
        for(i = 0; i < MAX_PORT_PER_SYSTEM; i++)//check if vlan flooding group exists
        {
            if ((pVpn->flooding.member_handle[i]) && (pVpnPort->bcmember == pVpn->flooding.member_handle[i]))
            {
                status = WP_IwMcMemberDelete(pVpn->flooding.group_handle, pVpnPort->bcmember);
                App_TerminateOnError(status, "WP_IwMcMemberDelete() bc member", __LINE__);
                
                pVpnPort->bcmember = 0;
                pVpn->flooding.member_handle[i] = 0;
                pVpn->flooding.count--;

                break;
            }
        }
        
        if (pVpnPort->mac_num)
        {
            WPE_DeleteFDBNodeByPort(pVpn, vpnport);
        }
    }

    if (PORT_TYPE_UNI == pVpnPort->type)
    {
        pVpn->uni_num--;
    }
    else
    if (PORT_TYPE_NNI == pVpnPort->type)
    {
         pVpn->nni_num--;
    }

    if (pVpnPort->vlanrule)
    {
        status = WP_PceRuleDelete(pVpnPort->vlanrule);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pVpnPort->vlanrule = 0;        
    }

    if (pVpnPort->learning)
    {
        status = WP_PceRuleDelete(pVpnPort->learning);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pVpnPort->learning = 0;        
    }

    if (pVpnPort->nextround)
    {
        status = WP_PceRuleDelete(pVpnPort->nextround);
        App_TerminateOnError(status, "WP_PceRuleDelete", __LINE__);
        pVpnPort->nextround = 0;                
    }

    if (pVpnPort->flowagg)
    {
        status = WP_IwFlowAggregationDelete(pVpnPort->flowagg);
        App_TerminateOnError(status, "WP_IwFlowAggregationDelete", __LINE__);
        pVpnPort->flowagg = 0;
    }

    if (pVpnPort->bport)
    {
        status = WP_IwPortDelete(pVpnPort->bport);
        App_TerminateOnError(status, "WP_IwPortDelete", __LINE__);
        pVpnPort->bport = 0;
    }
    
    pPort = hw_port + pVpnPort->portindex - 1;

    if (pVpnPort->subport)
    {
        pSubPort = pPort->subport + pVpnPort->subport - 1;
        pSubPort->num_vpnport--;
    }
    else
    {
        pPort->num_vpnport--;
    }

    // 

    // clear
    i = pVpnPort->index;
    memset(pVpnPort, 0, sizeof(WPE_TP_VPNPORT));
    pVpnPort->index = i;
    
    return 0;
}

Node* wpe_add_static_mac(WT_VsiSystemInfo* pVpn, Node mac)
{
    Node* temp = NULL;
    Node* pre = pVpn->static_mac;
    Node* current;
    if (NULL == pVpn->static_mac)
    {
        pVpn->static_mac = (Node*)malloc(sizeof(Node));
        if (NULL == pVpn->static_mac)
        {
            return NULL;
        }

        memcpy(pVpn->static_mac, &mac, sizeof(Node));
        pVpn->static_mac->next = NULL;

        return pVpn->static_mac;
    }

    temp = (Node*)malloc(sizeof(Node));
    if (NULL == temp)
    {
        return NULL;
    }

    memcpy(temp, &mac, sizeof(Node));
    temp->next = NULL;

    current = pre->next;
    while(current)
    {
        pre = current;
        current = current->next;
    }

    pre->next = temp;

    return temp;
}

int wpe_del_static_mac(WT_VsiSystemInfo* pVpn, Node mac)
{
    int ret = 0;
    WP_status status;
    Node* temp = NULL;
    Node* pre = pVpn->static_mac;
    Node* current;
    if (NULL == pVpn->static_mac)
    {
        return -1;
    }

    current = pVpn->static_mac;
    while(current)
    {
        if ((0 == memcmp(current->mac, mac.mac, 6)))
        {
            status = WP_PceRuleDelete(current->pce_rule);
            App_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);        
            if (current == pVpn->static_mac) 
            {
                pVpn->static_mac = current->next;
            }
            else 
            {
                pre->next = current->next;                    
            }
            ret = current->vpnport;
            temp = current;
            current = current->next;
            free(temp);
            return ret;
        }
        
        pre = current;
        current = current->next;
    }

    return 0;
}


void WPE_StaticMACShow(void)
{
    WP_U32 ii;
    WT_VsiSystemInfo* pVpn = vpn_system;
    Node* temp = NULL;

    printf("vpnid | vpn port | static | mac address \n");
    printf("-----------------------------------------------\n");
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++, pVpn++)
    {
        if ((!pVpn->valid) || (SYSTEM_TYPE_VSI != pVpn->type))
        {
            continue;
        }
        // static mac list
        temp = pVpn->static_mac;
        while(temp)
        {
        printf("  %2u       %3u       %1u      %02x:%02x:%02x:%02x:%02x:%02x\n", pVpn->index, temp->vpnport, temp->static_mac,
                    temp->mac[0], temp->mac[1], temp->mac[2], temp->mac[3], temp->mac[4], temp->mac[5]);
            temp = temp->next;
        }
        // learnt mac list
        temp = pVpn->Fdbhead;
        while(temp)
        {
        printf("  %2u       %3u       %1u      %02x:%02x:%02x:%02x:%02x:%02x\n", pVpn->index, temp->vpnport, temp->static_mac,
                    temp->mac[0], temp->mac[1], temp->mac[2], temp->mac[3], temp->mac[4], temp->mac[5]);
            temp = temp->next;
        }

    }

}



int WPE_StaticMACInsert(WP_U8 vpnid, WP_U8 port, WP_U8 d_mac[6])
{
    WP_handle rule;
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;
    WP_pce_rule_forwarding rule_fwd = {0};
    Node mac_add;

    if ((port < 1) || (port > NUM_OF_VPN_PORT) || (vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }
    
    pVpn = vpn_system + vpnid - 1;
    if ((!pVpn->valid) || (SYSTEM_TYPE_VSI != pVpn->type))
    {
        return -2;
    }

    pVpnPort = vpn_port + port - 1;
    if ((!pVpnPort->valid) || (vpnid != pVpnPort->vpnid))
    {
        return -3;
    }

    if (PORT_TYPE_UNI == pVpnPort->type)
    {
        rule_fwd.filter_handle = UNI_PCE_filter[FILTER_F11];
    }
    else
    if (PORT_TYPE_NNI == pVpnPort->type)
    {
        rule_fwd.filter_handle = NNI_PCE_filter[FILTER_F5];
    }
    else
    {
        return -4;
    }

    rule_fwd.enabled = WP_ENABLE;
    rule_fwd.match_action = WP_PCE_RULE_MATCH_ACCEPT;
    
    rule_fwd.rule_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    rule_fwd.rule_fields[0].value.iw_system_handle = pVpn->iw_system;

    rule_fwd.rule_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
    memcpy(rule_fwd.rule_fields[1].value.mac_addr, d_mac, 6);

    rule_fwd.rule_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    rule_fwd.match_result[0].result_type = WP_PCE_RESULT_OUT_IW_PORT_UPDATE;
    rule_fwd.match_result[0].param.iw_port.iw_port_handle = pVpnPort->bport;

    rule_fwd.match_result[1].result_type = WP_PCE_RESULT_AGING;
    rule_fwd.match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;    

    rule_fwd.match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
    rule_fwd.match_result[2].param.flow_agg.flow_aggregation = pVpnPort->flowagg;
    
    rule_fwd.match_result[3].result_type = WP_PCE_RESULT_LAST;

    rule = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_RULE_FORWARDING, &rule_fwd);
    App_TerminateOnError(rule, "WP_PceRuleCreate", __LINE__);

    mac_add.pce_rule = rule;
    mac_add.vpnport = port;
    mac_add.static_mac = 1;
    memcpy(mac_add.mac, d_mac, 6);
    
    if (NULL == wpe_add_static_mac(pVpn, mac_add))
    {

        return -6;
    }
    pVpnPort->item_num++;

    pVpn->path_num++;

    return 0;
}

int WPE_StaticMACDelete(WP_U8 vpnid, WP_U8 d_mac[6])
{
  
    int ret = 0;
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;
    
    Node mac_del;

    if ((vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }
    
    pVpn = vpn_system + vpnid - 1;
    if ((!pVpn->valid) || (SYSTEM_TYPE_VSI != pVpn->type))
    {
        return -2;
    }

    memcpy(mac_del.mac, d_mac, 6);
    ret = wpe_del_static_mac(pVpn, mac_del);

    if ( (ret <= 0)||(ret > NUM_OF_VPN_PORT))
    {
        return -4;
    }
    pVpnPort = vpn_port + ret - 1;
    if ((!pVpnPort->valid) || (vpnid != pVpnPort->vpnid))
    {
        return -5;
    }

    


    pVpnPort->item_num--;

    pVpn->path_num--;
    
    return 0;
}

WPE_IPV4_FLOW* wpe_AddLPMv4Flow(WT_VsiSystemInfo* pVpn, WPE_TP_VPNPORT* pVpnPort, WP_handle flow, WP_U32 prefix, WP_U32 dest_ip)
{
    WPE_IPV4_FLOW*  pre = pVpn->route;
    WPE_IPV4_FLOW*  current;
    WPE_IPV4_FLOW*  temp;
    if (NULL == pVpn->route)
    {
        pVpn->route = (WPE_IPV4_FLOW*)malloc(sizeof(WPE_IPV4_FLOW));
        if (NULL == pVpn->route)
        {
            return NULL;
        }

        pVpn->route->destip = dest_ip;
        pVpn->route->flow_handle = flow;
        pVpn->route->prefix = prefix;
        pVpn->route->vpnid = pVpnPort->vpnid;
        pVpn->route->vpnport = pVpnPort->index;
        pVpn->route->next = NULL;

        return pVpn->route;
    }

    temp = (WPE_IPV4_FLOW*)malloc(sizeof(WPE_IPV4_FLOW));
    if (NULL == temp)
    {
        return NULL;
    }

    temp->destip = dest_ip;
    temp->flow_handle = flow;
    temp->prefix = prefix;
    temp->vpnid = pVpnPort->vpnid;
    temp->vpnport = pVpnPort->index;
    temp->next = NULL;

    current = pre->next;
    while(current)
    {
        pre = current;
        current = current->next;
    }

    pre->next = temp;

    return temp;
}
int wpe_DelLPMv4Flow(WT_VsiSystemInfo* pVpn, WP_U32 prefix, WP_U32 dest_ip)
{
    int ret = 0;
    WP_status status;
    WPE_IPV4_FLOW* temp = NULL;
    WPE_IPV4_FLOW* pre = pVpn->route;
    WPE_IPV4_FLOW* current;
    if (NULL == pVpn->route)
    {
        return -1;
    }

    current = pVpn->route;
    while(current)
    {
        if ((prefix == current->prefix) && (dest_ip == current->destip))
        {
            status = WP_IwFlowDelete(pVpn->iw_system, current->flow_handle);
            App_TerminateOnError(status, "WP_IwFlowDelete",__LINE__);        
            if (current == pVpn->route) 
            {
                pVpn->route = current->next;
            }
            else 
            {
                pre->next = current->next;                    
            }

            ret = current->vpnport;
            
            temp = current;
            current=current->next;
            free(temp);
            return ret;
        }
        
        pre = current;
        current = current->next;
    }

    return 0;
}


void WPE_IPRouteShow(void)
{
    WP_U32 ii;
    WT_VsiSystemInfo* pVpn = vpn_system;
    WPE_IPV4_FLOW* temp = NULL;
    printf("vpnid | egress vpn port | dest ip address |   prefix    \n");
    printf("--------------------------------------------------------\n");

    for (ii = 0; ii < MAX_IW_SYSTEM; ii++, pVpn++)
    {
        if ((!pVpn->valid) || (SYSTEM_TYPE_VRF != pVpn->type) || (NULL == pVpn->route))
        {
            continue;
        }
        temp = pVpn->route;
        while(temp)
        {
        printf("  %2u          %3u         %03u.%03u.%03u.%03u   %03u.%03u.%03u.%03u\n", temp->vpnid, temp->vpnport, 
                    ((temp->destip >> 24) & 0xff), ((temp->destip >> 16) & 0xff), ((temp->destip >> 8) & 0xff), (temp->destip & 0xff),
                    ((temp->prefix >> 24) & 0xff), ((temp->prefix >> 16) & 0xff), ((temp->prefix >> 8) & 0xff), (temp->prefix & 0xff));
            temp = temp->next;
        }          
    }
  
}

int WPE_IPRouteCreate(WP_U8 vpnid, WP_U8 port, WP_U32 prefix, WP_U32 dest_ip)
{
    WP_status status;
    WP_handle flow;
    WP_flow_route_ipv4 route_cfg;
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;

    if ((port < 1) || (port > NUM_OF_VPN_PORT) || (vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }

    if ((0 == dest_ip ) || (0 == prefix))
    {
        return -4;
    }
    
    pVpn = vpn_system + vpnid - 1;
    if ((!pVpn->valid) || (SYSTEM_TYPE_VRF != pVpn->type))
    {
        return -2;
    }

    pVpnPort = vpn_port + port - 1;
    if ((!pVpnPort->valid) || (vpnid != pVpnPort->vpnid))
    {
        return -3;
    }

    dest_ip &= prefix;
    if (0 == dest_ip)
    {
        return -4;
    }
    route_cfg.ip_address    = dest_ip;
    route_cfg.mask          = prefix;
    route_cfg.aggregation   = pVpnPort->flowagg;
    route_cfg.programmable_prefix_value = 0;
    route_cfg.process_mode = WP_FLOW_ROUTE_PROCESS_PASS;

    flow = WP_IwFlowAdd(pVpn->iw_system, WP_FLOW_BRIDGE_LPM_IPV4, &route_cfg);
    App_TerminateOnError(flow, "WP_IwFlowAdd", __LINE__);

    if (NULL == wpe_AddLPMv4Flow(pVpn, pVpnPort, flow, prefix, dest_ip))
    {
        
        return -6;        
    }

    status = WP_IwSystemBuild(pVpn->iw_system);
    App_TerminateOnError(status, "WP_IwSystemBuild", __LINE__);

    pVpn->path_num++;
    pVpnPort->item_num++;
    
    return 0;
}


int WPE_IPRouteDelete(WP_U8 vpnid, WP_U32 prefix, WP_U32 dest_ip)
{
    int ret = 0;
    WP_status status;
    WT_VsiSystemInfo* pVpn = NULL;
    WPE_TP_VPNPORT* pVpnPort = NULL;

    if ((vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }
    
    pVpn = vpn_system + vpnid - 1;
    if ((!pVpn->valid) || (SYSTEM_TYPE_VRF != pVpn->type))
    {
        return -2;
    }

    dest_ip &= prefix;
    if (0 == dest_ip)
    {
        return -3;
    }    
    ret = wpe_DelLPMv4Flow(pVpn, prefix, dest_ip);

    if ( (ret <= 0)||(ret > NUM_OF_VPN_PORT))
    {
        return -4;
    }

    status = WP_IwSystemBuild(pVpn->iw_system);
    App_TerminateOnError(status, "WP_IwSystemBuild", __LINE__);

    pVpnPort = vpn_port + ret - 1;
    if ((!pVpnPort->valid) || (vpnid != pVpnPort->vpnid))
    {
        return -5;
    }
   
    pVpnPort->item_num--;

    pVpn->path_num--;
    
    return 0;
}



WPE_IN_POLICER* WPE_PolicerFind(WP_handle handle)
{
    int i;
    for (i = 0; i < NUM_OF_POLICER; i++)
    {
        if (ing_policer[i].handle == handle)
        {
            return &ing_policer[i];
        }
    }

    return NULL;
}

void WPE_PolicerShow(void)
{
    int i;
    printf(" vpnport | CIR(Mbps) | EIR(Mbps) | CBS(bytes) | EBS(bytes) | mode\n");
    printf("-----------------------------------------------------------------\n");
    for (i = 0; i < NUM_OF_POLICER; i++)
    {
        if (ing_policer[i].handle)
        {
    printf("   %3u     %8u    %8u    %8u    %8u      %1u\n", ing_policer[i].vpnport, 
                    ing_policer[i].v2policer.cir, ing_policer[i].v2policer.eir, 
                    ing_policer[i].v2policer.cbs, ing_policer[i].v2policer.ebs, 
                    ing_policer[i].v2policer.color_mode);            
        }
    }

}

static WP_policer_v2 ing_pv2_cfg =
{
#if POLICER_HIER
        /* WP_policer_method method; */ WP_POLICER_METHOD_MEF_SHARING_WITH_COUPLING_FLAG,
#else
        /* WP_policer_method method; */ WP_POLICER_METHOD_MEF_WITH_COUPLING_FLAG,
#endif    
#if POLICER_TYPE_INT
        /* WP_policer_v2_type type; */  WP_POLICER_V2_TYPE_INTERNAL,

#else
        /* WP_policer_v2_type type; */  WP_POLICER_V2_TYPE_EXTERNAL,
#endif

        /* color_mode */        WP_POLICER_COLOR_BLIND,
        /* rd_packets_action */ WP_POLICER_DROP_RED_PACKETS,
        /* cir */               (700000000/8),  /* bytes/sec */
        /* cbs */               81920, /* need to check with shaping at l2/l1 groups */
        /* eir */               (300000000/8),  /* bytes/sec */
        /* ebs */               81920,
        /* policer_action */    0,
        /* frame_len_correction */  WP_POLICER_LENGTH_CORRECT_4B,
        /* frame_len_crct_action */ WP_POLICER_FRAME_LEN_CORRECTION_ADD,
#if 0
        /* WP_U32 cir_max */            10485760, /* (0xA0-0000) 10G value */
        /* WP_U32 eir_max */            20971520, /* eir_max >= cir+eir */
        /* WP_handle hierarchical_group_handle */ WP_POLICER_GROUP_UNUSED, /* Group index will be set after GroupCreate */
        /* WP_U32 hierarchical_ranking */ WP_POLICER_RANK_UNUSED, /* Priority is set in PolicerCreate */
#endif
};

int WPE_PolicerCreate(WP_U16 vpnport, WP_U32 cir, WP_U32 eir, WP_U32 cbs, WP_U32 ebs, WP_U8 color_mode)
{
    WP_status status;
    WP_pce_rule_classification rule_cfg = {0};
    WPE_TP_VPNPORT* pVPNPort = NULL;
    WT_VsiSystemInfo* pVpn = NULL;


    if ((vpnport < 1) || (vpnport > NUM_OF_VPN_PORT))
    {
        return -1;
    }

    if (0 == cir)
    {
        printf("Invalid CIR.\n");
            return -4;
    }
    
    if ((cir * WT_MEGA) > MAX_RATE)
    {
        printf("CIR exceed MAX rate %ubps.\n", MAX_RATE);
            return -4;            
    }


    pVPNPort = vpn_port + vpnport - 1;
    if ((!pVPNPort->valid) || (PORT_TYPE_UNI != pVPNPort->type))
    {
        return -2;
    }

    ing_pv2_cfg.cir = cir * WT_MEGA / 8;
    ing_pv2_cfg.eir = eir * WT_MEGA / 8;
    ing_pv2_cfg.cbs = cbs;
    ing_pv2_cfg.ebs = ebs;
    ing_pv2_cfg.color_mode = color_mode ? WP_POLICER_COLOR_AWARE : WP_POLICER_COLOR_BLIND;

    if (pVPNPort->policer)
    {
        if (0 == memcmp(&pVPNPort->policer->v2policer, &ing_pv2_cfg, sizeof(ing_pv2_cfg)))
        {
            return 0;
        }

        status = WP_PolicerModify(pVPNPort->policer->handle, WP_POLICER_MOD_ALL, &ing_pv2_cfg);
        App_TerminateOnError(status, "WP_PolicerModify", __LINE__);
        memcpy(&pVPNPort->policer->v2policer, &ing_pv2_cfg, sizeof(ing_pv2_cfg));
        
        pVPNPort->policer->v2policer.cir = cir;
        pVPNPort->policer->v2policer.eir = eir;

        return 0;
    }

    pVPNPort->policer = WPE_PolicerFind(0);
    if (NULL == pVPNPort->policer)
    {
        return -8;
    }

    pVPNPort->policer->handle = WP_PolicerCreate(WP_WINPATH(0), WP_POLICER_V2, &ing_pv2_cfg);
    App_TerminateOnError(pVPNPort->policer->handle, "WP_PolicerCreate() - Ingress", __LINE__);

    // update pce result

    pVpn = vpn_system + pVPNPort->vpnid - 1;
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        if ((pVpn->pUNI) && (pVpn->pNNI) && (pVpn->pUNI->vpwsrule))
        {
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpn->pUNI->bport;

            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[1].param.flow_agg.flow_aggregation = pVpn->pNNI->flowagg;

            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INGRESS_POLICER;
            rule_cfg.match_result[2].param.ingress_policer.policer = pVpn->pUNI->policer->handle;
            
            rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;
            status = WP_PceRuleModify(pVpn->pUNI->vpwsrule, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
            
        }
    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVPNPort->bport;

        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
        rule_cfg.match_result[1].param.learning_flow_agg.flow_aggregation = pVPNPort->flowagg;                        
        
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_INGRESS_POLICER;
        rule_cfg.match_result[2].param.ingress_policer.policer = pVPNPort->policer->handle;
        rule_cfg.match_result[3].result_type = WP_PCE_RESULT_LAST;            

        status = WP_PceRuleModify(pVPNPort->learning, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
        App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
    }
    else
    if (SYSTEM_TYPE_VRF == pVpn->type)
    {
        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVPNPort->bport;
    
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_INGRESS_POLICER;
        rule_cfg.match_result[1].param.ingress_policer.policer = pVPNPort->policer->handle;
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
        status = WP_PceRuleModify(pVPNPort->learning, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
        App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
    }

    memcpy(&pVPNPort->policer->v2policer, &ing_pv2_cfg, sizeof(ing_pv2_cfg));
    pVPNPort->policer->v2policer.cir = cir;
    pVPNPort->policer->v2policer.eir = eir;
    pVPNPort->policer->vpnport = vpnport;

    return 0;
}

int WPE_PolicerDelete(WP_U16 vpnport)
{
    WP_status status;
    WPE_TP_VPNPORT* pVPNPort = NULL;
    WT_VsiSystemInfo* pVpn = NULL;
    WP_pce_rule_classification rule_cfg = {0};

    if ((vpnport < 1) || (vpnport > NUM_OF_VPN_PORT))
    {
        return -1;
    }

    pVPNPort = vpn_port + vpnport - 1;

    if ((!pVPNPort->valid) || (PORT_TYPE_UNI != pVPNPort->type) || (NULL == pVPNPort->policer) || (0 == pVPNPort->policer->handle))
    {
        return -2;
    }
    
    pVpn = vpn_system + pVPNPort->vpnid - 1;
    if (SYSTEM_TYPE_VPWS == pVpn->type)
    {
        if ((pVpn->pUNI) && (pVpn->pNNI) && (pVpn->pUNI->vpwsrule))
        {
            rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
            rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVpn->pUNI->bport;

            rule_cfg.match_result[1].result_type = WP_PCE_RESULT_FLOW_AGG;
            rule_cfg.match_result[1].param.flow_agg.flow_aggregation = pVpn->pNNI->flowagg;
            
            rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;
            status = WP_PceRuleModify(pVpn->pUNI->vpwsrule, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
            App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
            
        }
    }
    else
    if (SYSTEM_TYPE_VSI == pVpn->type)
    {
        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVPNPort->bport;
        
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LEARNING_FLOW_AGG;
        rule_cfg.match_result[1].param.learning_flow_agg.flow_aggregation = pVPNPort->flowagg;                        
        
        rule_cfg.match_result[2].result_type = WP_PCE_RESULT_LAST;            
        
        status = WP_PceRuleModify(pVPNPort->learning, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
        App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
    }
    else
    if (SYSTEM_TYPE_VRF == pVpn->type)
    {
        rule_cfg.match_result[0].result_type = WP_PCE_RESULT_IN_IW_PORT_UPDATE;
        rule_cfg.match_result[0].param.iw_port.iw_port_handle = pVPNPort->bport;
    
        rule_cfg.match_result[1].result_type = WP_PCE_RESULT_LAST;
        status = WP_PceRuleModify(pVPNPort->learning, WP_PCE_RULE_MODIFY_COMMAND_RESULT, &rule_cfg);
        App_TerminateOnError(status, "WP_PceRuleModify", __LINE__);
    }

    status = WP_PolicerDelete(pVPNPort->policer->handle);
    App_TerminateOnError(status, "WP_PolicerDelete()", __LINE__);
    memset(pVPNPort->policer, 0, sizeof(WPE_IN_POLICER));
    pVPNPort->policer = NULL;

    return 0;
}

/*
typedef enum
{
   WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE,
   WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL,
   WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO,
   WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP,
   WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_PRIO,
   WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_DEI,
   WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_PRIO_DEI,
   WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_PRIO,
   WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_DEI,
   WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_PRIO_DEI,
   WP_IW_GPMT_REMARKING_TYPE_TOS_DSCP,
   WP_IW_GPMT_REMARKING_TYPE_TOS_PRECEDENCE,
   WP_IW_GPMT_REMARKING_TYPE_TOS_DSCP_DROP_PRECEDENCE,
   WP_IW_GPMT_REMARKING_TYPE_TOS,
}WP_iw_gpmt_prefix_remarking_type;

*/
char *wpe_Prefix_Remarking_Type2string(WP_iw_gpmt_prefix_remarking_type type)
{

    switch (type)
    {
        case WP_IW_GPMT_REMARKING_TYPE_DO_NOT_CHANGE:           return "DO NOT CHANGE    "; break;
        case WP_IW_GPMT_REMARKING_TYPE_PREFIX_REMARKING_VAL:    return "PREFIX VAL       "; break;
        case WP_IW_GPMT_REMARKING_TYPE_PREFIX_VLAN_PRIO:        return "PREFIX VLAN PRIO "; break;
        case WP_IW_GPMT_REMARKING_TYPE_PREFIX_MPLS_EXP:         return "PREFIX MPLS EXP  "; break;
        case WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_PRIO:           return "EXT VLAN PRIO    "; break;
        case WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_DEI:            return "EXT VLAN DEI     "; break;
        case WP_IW_GPMT_REMARKING_TYPE_EXT_VLAN_PRIO_DEI:       return "EXT VLAN PRIO DEI"; break;
        case WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_PRIO:           return "INT VLAN PRIO    "; break;
        case WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_DEI:            return "INT VLAN DEI     "; break;
        case WP_IW_GPMT_REMARKING_TYPE_INT_VLAN_PRIO_DEI:       return "INT VLAN PRIO DEI"; break;
        case WP_IW_GPMT_REMARKING_TYPE_TOS_DSCP:                return "TOS DSCP         "; break;
        case WP_IW_GPMT_REMARKING_TYPE_TOS_PRECEDENCE:          return "TOS PRECEDENCE   "; break;
        case WP_IW_GPMT_REMARKING_TYPE_TOS_DSCP_DROP_PRECEDENCE:return "DSCP DROP PRECEDE"; break;
        case WP_IW_GPMT_REMARKING_TYPE_TOS:                     return "TOS              "; break;
        
        default:                                                return "???              "; break;    
    }

}

void WPE_GPMT_Show(void)
{
    WP_U32 ii, jj;
    WP_iw_gpmt* pIwGpmt = iw_gpmt_cfg;
    WP_iw_gpmt_priority_params * pParam;
    for (ii = 0; ii < MAX_GPMT_TABLE; ii++, pIwGpmt++)
    {
        printf("\nGPMT index: %u,  entry num: %u,  handle: 0x%x\n", ii, pIwGpmt->num_of_priority_group_entries, iw_gpmt_table_handle[ii]);
        if (0 == pIwGpmt->num_of_priority_group_entries)
        {
            continue;
        }
        printf("-----------------------------------------------------------------------\n");
        printf("priority| color |priority mapping value|   remark array  | remark value\n");
        printf("-----------------------------------------------------------------------\n");

        for (jj = 0; jj < pIwGpmt->num_of_priority_group_entries; jj++)
        {
            pParam = pIwGpmt->iw_gpmt_priority_entry + jj;
            printf("   %2u     %s           %2u           - %s 0x%2x\n", jj, "green ",  
                            pParam->green.priority_mapping_value,
                            wpe_Prefix_Remarking_Type2string(pParam->green.prefix_remarking_data.remarking_type[0]), 
                            pParam->green.prefix_remarking_data.value[0]);
            printf("                                        - %s 0x%2x\n", 
                            wpe_Prefix_Remarking_Type2string(pParam->green.prefix_remarking_data.remarking_type[1]), 
                            pParam->green.prefix_remarking_data.value[1]);
            printf("                                        - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->green.prefix_remarking_data.remarking_type[2]), 
                            pParam->green.prefix_remarking_data.value[2]);                            
            printf("                                        - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->green.prefix_remarking_data.remarking_type[3]), 
                            pParam->green.prefix_remarking_data.value[3]);
            printf("\n");                            
            printf("   %2u     %s           %2u           - %s 0x%2x\n", jj, "yellow", 
                            pParam->yellow.priority_mapping_value, 
                            wpe_Prefix_Remarking_Type2string(pParam->yellow.prefix_remarking_data.remarking_type[0]), 
                            pParam->yellow.prefix_remarking_data.value[0]);
            printf("                                        - %s 0x%2x\n", 
                            wpe_Prefix_Remarking_Type2string(pParam->yellow.prefix_remarking_data.remarking_type[1]), 
                            pParam->yellow.prefix_remarking_data.value[1]);
            printf("                                        - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->yellow.prefix_remarking_data.remarking_type[2]), 
                            pParam->yellow.prefix_remarking_data.value[2]);                            
            printf("                                        - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->yellow.prefix_remarking_data.remarking_type[3]), 
                            pParam->yellow.prefix_remarking_data.value[3]);
/*                            
            printf("   %2u     %s    %2u     0x%2x   - %s 0x%2x\n", jj, " red  ", pParam->red.priority_mapping_value, 
                            pParam->red.action_mode, 
                            wpe_Prefix_Remarking_Type2string(pParam->red.prefix_remarking_data.remarking_type[0]), 
                            pParam->red.prefix_remarking_data.value[0]);
            printf("                                  - %s 0x%2x\n", 
                            wpe_Prefix_Remarking_Type2string(pParam->red.prefix_remarking_data.remarking_type[1]), 
                            pParam->red.prefix_remarking_data.value[1]);
            printf("                                  - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->red.prefix_remarking_data.remarking_type[2]), 
                            pParam->red.prefix_remarking_data.value[2]);                            
            printf("                                  - %s 0x%2x\n",
                            wpe_Prefix_Remarking_Type2string(pParam->red.prefix_remarking_data.remarking_type[3]), 
                            pParam->red.prefix_remarking_data.value[3]);
*/                            
            printf("\n");
        }
        
    }
}


WP_U8 charbuff[8192];
WP_display_destination destination =
{
   /*echo_stdout  */ 1,
   /*file_name    */ 0,
   /*memory_start */ charbuff,
   /*memory_size  */ 4096
};

void WPE_Display(WP_U32 handle, WP_U32 param)
{
    WP_Display(0, handle, param, &destination);

}


