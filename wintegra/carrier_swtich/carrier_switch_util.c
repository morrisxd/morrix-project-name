 /*****************************************************************************
 * (C) Copyright PMC - WIN division (Wintegra) 2013.  All rights reserved.
 * PMC-Sierra (WINTEGRA) CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of PMC-Sierra (Wintegra).
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

#include "carrier_switch_util.h"

#ifdef PRINT_STAT_FIELD
#undef PRINT_STAT_FIELD
#endif
#define PRINT_STAT_FIELD(field) printf( "%08x%08x", (WP_U32)(field>>32), (WP_U32)field)



void WPE_Pecs_Init(WP_gpe_pecs gpe_pecs_config[],WP_handle pecs_handle[])
{
        WP_U32 ii;
   
        for (ii = 0 ; ii < NUM_OF_PECS ; ii++)
        {
                /* It loads and relocates the PECS DPS image from the predefined PECS library
                 * to the DPS program RAM. It also registers the PECS Service Function.
                 */
                pecs_handle[ii] = WP_GpePecsCreate(WP_WINPATH(DEFAULT_WPID),WP_GPE_PECS,(void*)&gpe_pecs_config[ii]);
                printf("CREATING PECS: handle returned: %x\n", pecs_handle[ii]);
                App_TerminateOnError(pecs_handle[ii],"WP_GpePecsCreate", __LINE__);  
        }
}

void F_MyAppIndicateEventIwgpLearning(WP_tag tag, WP_U32 data, WP_U32 info)
{
#if LEARNING_MODE_INTERRUPT
    if (learning_enable)
    {
        /* Tell the Learning thread there are rules need to learn */
        WPL_SemIncrement(&learning_sem, 1);
    }
#endif
        
    return ;
}

WP_boolean WPE_HWPortIdValid(WP_U8 port)
{
        if (port >= NR_GBE)
        {
                return WP_FALSE;
        }

        if ((PORT_HW_XGI == hw_port[port].hw_type)
            || (PORT_HW_SGMII == hw_port[port].hw_type))
        {
                return WP_TRUE;
        }

        return WP_FALSE;

}


void WPE_PortListDisplay(void)
{
        WP_U8 i;
        printf("Port index:\n");
        for (i = 0; i < NR_GBE; i++)
        {
                if (PORT_HW_XGI == hw_port[i].hw_type)
                {
                        printf("%u-XGI%u; ", hw_port[i].index, (hw_port[i].hw_port + 1 - WP_PORT_XGI1));
                }
                else
                if (PORT_HW_SGMII == hw_port[i].hw_type)
                {
                        printf("%u-ENET%u; ", hw_port[i].index, (hw_port[i].hw_port + 1 - WP_PORT_ENET1));
                }    
        }
        printf("\n");
}


void WPE_CreatePceFilters(void)
{
    WP_pce_filter_classification filter = {0};
    WP_pce_filter_forwarding fwd_filter_cfg = {0};
    WP_pce_filter_learning lrn_filter_cfg = {0};

    /*    
        F1: Subport VLAN
        Key: Iw port+subport VLAN
        M. Result: Update In IW Port
        NMA:Deny NFA:Cont
    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_SF_SPVLAN_TPID;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    
    filter.filter_fields[1].mask.spvlan_tpids.spvlan_tag = WP_PCE_MASK_VLAN_ID;
    filter.filter_fields[1].mask.spvlan_tpids.int_tpid = 0;
    filter.filter_fields[1].mask.spvlan_tpids.ext_tpid = 0;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F1] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F1], "WP_PceFilterCreate",__LINE__);


    /* 
        F2: Int VLAN pri  maping
        Key: iw port+ INT vlan pri+vlan type*
        M. Result: Update priority mapping
        NMA:Cont NFA:Cont
    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    
    filter.filter_fields[1].field_id   = WP_PCE_FIELD_ID_INT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_PRIORITY;

    filter.filter_fields[2].field_id   = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_MASK;
    
    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F2] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F2], "WP_PceFilterCreate",__LINE__);

    /* 
        F3: Ext VLAN pri mapping
        Key: iw port+ Ext vlan pri+vlan type*
        M. Result: Update priority mapping
        NMA:Cont NFA:Cont
    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_PRIORITY;

    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_MASK;
    
    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
    
    UNI_PCE_filter[FILTER_F3] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F3], "WP_PceFilterCreate",__LINE__);


    /* 
        F4: VPN port mapping(double tag)
        Key: EXT+INT VID+In IW Port+vlan_type*
        M. Result: update In IW Port+learning
        flow_agg/flow_agg for vpws + policer NMA:Deny  NFA:Cont

    */
    //filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[3].mask.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_MASK;
    
    filter.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
    
    UNI_PCE_filter[FILTER_F4] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F4], "WP_PceFilterCreate",__LINE__);



    /*     
        F5: VPN port mapping(single tag) 
        Key:INT VID+In IW Port+vlan_type*
        M. Result: update In IW Port+learning
        flow_agg/flow_agg for vpws+policer  NMA:Deny  NFA:Deny
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

    */    
        filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
        filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_MASK;
    
    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F5] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION,
                                                                          &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F5], "WP_PceFilterCreate",__LINE__);
                                                                          
    /*
        F6: vlan translation(double tag)
        Key: EXT+INT VID+In IW Port+vlan_type*
        M. Result: update Ext+INT VID NMA:Cont NFA:Cont

    */
    
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_EXT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;

    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[3].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[3].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[3].mask.vlan_type = PCE_RULE_VLAN_TYPE_2TAG_MASK;
    
    filter.filter_fields[4].field_id = WP_PCE_FIELD_ID_LAST;
    
    UNI_PCE_filter[FILTER_F6] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F6], "WP_PceFilterCreate",__LINE__);


    
    /* 
        F7: vlan translation(single tag) 
        Key:INT VID+In IW Port+vlan_type*
        M. Result: update INT VID NMA:Cont  NFA:Cont

    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
    
    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
    
    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    
    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_INT_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_ID;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_VLAN_TYPE;
    filter.filter_fields[2].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[2].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[2].mask.vlan_type = PCE_RULE_VLAN_TYPE_1TAG_MASK;
    
    filter.filter_fields[3].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F7] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION,
                                                                          &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F7], "WP_PceFilterCreate",__LINE__);

    /*    
        F8: priority mapping table index
        Key: IW System ID 
        M. Result: GPMT index** (cont  for VSI and 
        Accept for VRF)
        NMA:Cont NFA:Cont

    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
    
    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
    
    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;   
    
    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F8] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION,
                                                                          &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F8], "WP_PceFilterCreate",__LINE__);

    

    /* 
        F9: VSI flooding
        Key: IW System ID + UC& BC (parser flags)
        M. Result: flooding group(MC FA)
        NMA:Deny NFA:Cont

    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;
    
    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
    
    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;   

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED; 
    filter.filter_fields[1].mask.parser_flags = WP_PCE_MASK_PARSER_FLAG_MC;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    UNI_PCE_filter[FILTER_F9] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION,
                                                                          &filter);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F9], "WP_PceFilterCreate",__LINE__); 

    /*
        F11: Forwarding
        Key: IW System ID + MAC-DA
        M. Result: forwarding FA
        NMA:Cont NFA:Deny

    */
    fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;
    
    fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;
    
    fwd_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
    fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    
    fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    
    UNI_PCE_filter[FILTER_F11] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_FORWARDING, &fwd_filter_cfg);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F11], "WP_PceFilterCreate",__LINE__);
    
    /* 
        F10: Learning
        Key: IW System ID + MAC-SA
        M. Result: N/A
    */    
    lrn_filter_cfg.forwarding_filter = UNI_PCE_filter[FILTER_F11];
    lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

    lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    UNI_PCE_filter[FILTER_F10] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID),
                                                         WP_PCE_FILTER_LEARNING,
                                                         &lrn_filter_cfg);
    App_TerminateOnError(UNI_PCE_filter[FILTER_F10], "WP_PceFilterCreate",__LINE__);



    /* NNI */

    memset(&filter, 0, sizeof(filter));
    memset(&fwd_filter_cfg, 0, sizeof(fwd_filter_cfg));
    memset(&lrn_filter_cfg, 0, sizeof(lrn_filter_cfg));

    /*
        F1: VLAN pri
        Key: iw port+vlan pri
        M. Result: Update priority mapping
    */
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    
    filter.filter_fields[1].field_id   = WP_PCE_FIELD_ID_VLAN_TAG;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.vlan_tag = WP_PCE_FIELD_MASK_VLAN_PRIORITY;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    NNI_PCE_filter[FILTER_F1] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F1], "WP_PceFilterCreate",__LINE__);

    /*
    
        F2: MPLS Label
        Key: Iw port+PWE label
        M. Result: update In IW Port + PCE i/f in case of L2/L3 VPN
        flow_agg  for VPWS**. 
         NMA: Deny NFA:Deny
    */

    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_INPUT_IW_PORT;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;
    
    filter.filter_fields[1].field_id   = WP_PCE_FIELD_ID_MPLS_HEADER1;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.mpls_label = WP_PCE_MASK_MPLS_LABEL;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;
    NNI_PCE_filter[FILTER_F2] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F2], "WP_PceFilterCreate",__LINE__); 


    /*    
        F1: Unknown Unicast
        Key: IW System ID + UC& BC (parser flags)
        M. Result: flooding group(MC FA)
        NMA:Deny NFA:Cont

    */
/*    
    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_DENY;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_PARSER_FLAGS;
    filter.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_USED;
    filter.filter_fields[1].mask.parser_flags = WP_PCE_MASK_PARSER_FLAG_MC;
    
    filter.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    NNI_PCE_filter[FILTER_F3] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F3], "WP_PceFilterCreate",__LINE__);

    */
    NNI_PCE_filter[FILTER_F3] = UNI_PCE_filter[FILTER_F9];
    /* 
        F3: Forwarding
        Key: IW System ID + MAC-DA
        M. Result: forwarding FA
        NMA:Cont NFA:Deny
    */
    /*
    fwd_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    fwd_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_DENY; //WP_PCE_FILTER_NO_FIELDS_DENY;

    fwd_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    fwd_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    fwd_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    fwd_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    fwd_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_DA;
    fwd_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    fwd_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    fwd_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;


    NNI_PCE_filter[FILTER_F5] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID),
                                                           WP_PCE_FILTER_FORWARDING,
                                                           &fwd_filter_cfg);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F5], "WP_PceFilterCreate",__LINE__);
    
    */
    NNI_PCE_filter[FILTER_F5] = UNI_PCE_filter[FILTER_F11];

    /* 
        F2: Learning
        Key: IW System ID + MAC-SA
        M. Result: N/A
    */
    /*
    lrn_filter_cfg.forwarding_filter = NNI_PCE_filter[FILTER_F5];
    lrn_filter_cfg.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    lrn_filter_cfg.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE; //WP_PCE_FILTER_NO_FIELDS_DENY;

    lrn_filter_cfg.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    lrn_filter_cfg.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    lrn_filter_cfg.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    lrn_filter_cfg.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    lrn_filter_cfg.filter_fields[1].field_id = WP_PCE_FIELD_ID_MAC_SA;
    lrn_filter_cfg.filter_fields[1].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    lrn_filter_cfg.filter_fields[1].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    lrn_filter_cfg.filter_fields[2].field_id = WP_PCE_FIELD_ID_LAST;

    NNI_PCE_filter[FILTER_F4] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID),
                                                         WP_PCE_FILTER_LEARNING,
                                                         &lrn_filter_cfg);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F4], "WP_PceFilterCreate",__LINE__);
   
    */
    NNI_PCE_filter[FILTER_F4] = UNI_PCE_filter[FILTER_F10];

    filter.no_match_action = WP_PCE_FILTER_NO_MATCH_CONTINUE;
    filter.no_fields_action = WP_PCE_FILTER_NO_FIELDS_CONTINUE;

    filter.no_match_result[0].result_type = WP_PCE_RESULT_LAST;

    filter.filter_fields[0].field_id = WP_PCE_FIELD_ID_IW_SYSTEM;
    filter.filter_fields[0].field_mode = WP_PCE_FIELD_MODE_COMPARE_EXACT_MATCH;
    filter.filter_fields[0].mask_mode = WP_PCE_FIELD_MASK_NOT_USED;

    filter.filter_fields[1].field_id = WP_PCE_FIELD_ID_LAST;

    NNI_PCE_filter[FILTER_F6] = WP_PceFilterCreate(WP_WINPATH(DEFAULT_WPID), WP_PCE_FILTER_CLASSIFICATION, &filter);
    App_TerminateOnError(NNI_PCE_filter[FILTER_F6], "WP_PceFilterCreate",__LINE__);
    
}


void WPE_CreatePceFilterSets(void)
{
    WP_pce_filter_set fs_level;

    fs_level.filter_set_level = 1;
    fs_level.next_filter_set = WP_UNUSED;
    fs_level.filters[0] = NNI_PCE_filter[FILTER_F3];        
    fs_level.filters[1] = NNI_PCE_filter[FILTER_F4];
    fs_level.filters[2] = NNI_PCE_filter[FILTER_F5];        
    fs_level.filters[3] = WP_UNUSED;

    filter_set_vpls = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level);
    App_TerminateOnError(filter_set_vpls, "WP_PceFilterSetCreate",__LINE__);

    /* filter set with learning */
    fs_level.filter_set_level = 0;
    fs_level.next_filter_set = filter_set_vpls;
    fs_level.filters[0] = UNI_PCE_filter[FILTER_F1];    
    fs_level.filters[1] = UNI_PCE_filter[FILTER_F2];
    fs_level.filters[2] = UNI_PCE_filter[FILTER_F3];
    fs_level.filters[3] = UNI_PCE_filter[FILTER_F4];
    fs_level.filters[4] = UNI_PCE_filter[FILTER_F5];
    fs_level.filters[5] = UNI_PCE_filter[FILTER_F6];
    fs_level.filters[6] = UNI_PCE_filter[FILTER_F7];
    fs_level.filters[7] = UNI_PCE_filter[FILTER_F8];
    fs_level.filters[8] = WP_UNUSED;
/*    
    fs_level.filters[8] = UNI_PCE_filter[FILTER_F9];
    fs_level.filters[9] = UNI_PCE_filter[FILTER_F10];
    fs_level.filters[10] = UNI_PCE_filter[FILTER_F11];
    fs_level.filters[11] = WP_UNUSED;
*/
    filter_set_uni_subport = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level);
    App_TerminateOnError(filter_set_uni_subport, "WP_PceFilterSetCreate",__LINE__);

    fs_level.filter_set_level = 0;
    fs_level.next_filter_set = filter_set_vpls;
    fs_level.filters[0] = UNI_PCE_filter[FILTER_F2];
    fs_level.filters[1] = UNI_PCE_filter[FILTER_F3];
    fs_level.filters[2] = UNI_PCE_filter[FILTER_F4];
    fs_level.filters[3] = UNI_PCE_filter[FILTER_F5];
    fs_level.filters[4] = UNI_PCE_filter[FILTER_F6];
    fs_level.filters[5] = UNI_PCE_filter[FILTER_F7];
    fs_level.filters[6] = UNI_PCE_filter[FILTER_F8];
    fs_level.filters[7] = WP_UNUSED;

/*    
    fs_level.filters[7] = UNI_PCE_filter[FILTER_F9];
    fs_level.filters[8] = UNI_PCE_filter[FILTER_F10];
    fs_level.filters[9] = UNI_PCE_filter[FILTER_F11];
    fs_level.filters[10] = WP_UNUSED;
*/
    filter_set_uni_no_subport = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level);
    App_TerminateOnError(filter_set_uni_no_subport, "WP_PceFilterSetCreate",__LINE__);


    fs_level.filter_set_level = 0;
    fs_level.next_filter_set = WP_UNUSED;
    fs_level.filters[0] = NNI_PCE_filter[FILTER_F1];        
    fs_level.filters[1] = NNI_PCE_filter[FILTER_F2];
    fs_level.filters[2] = WP_UNUSED;

    filter_set_nni_pre = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level);
    App_TerminateOnError(filter_set_nni_pre, "WP_PceFilterSetCreate",__LINE__);


    fs_level.filter_set_level = 0;
    fs_level.next_filter_set = WP_UNUSED;
    fs_level.filters[0] = NNI_PCE_filter[FILTER_F6];        
    fs_level.filters[1] = WP_UNUSED;

    filter_set_nni_vrf = WP_PceFilterSetCreate(WP_WINPATH(DEFAULT_WPID), &fs_level);
    App_TerminateOnError(filter_set_nni_vrf, "WP_PceFilterSetCreate",__LINE__);

    filter_set_nni_vpws = filter_set_nni_vrf;

}

void WPE_CreatePceInterface(WP_handle iw_system, WP_handle rx_channel, WP_handle filter_set, WP_boolean subport_en)
{
    WP_pce_if_params_pkt_rx_channel pce_if_params = {0};
    WP_handle pce_if_handle;
    WP_status status;

    pce_if_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
    pce_if_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;
    pce_if_params.ip_header_validation = WP_DISABLE;
    pce_if_params.sub_port_vlan = subport_en ? WP_ENABLE : WP_DISABLE;
    pce_if_params.collect_tpids = subport_en ? WP_ENABLE : WP_DISABLE;

    pce_if_params.filter_set_handle = filter_set;

    pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_PKT_RX_CHANNEL, &pce_if_params);
    App_TerminateOnError(pce_if_handle, "WP_PceInterfaceCreate RX CHANNEL", __LINE__);

    /* Modify the PCE interface for GE RX channel handle */
    rx_binding_cfg[0].pce_if_handle = pce_if_handle;
    status = WP_IwRxBindingModify(rx_channel,
                                  iw_system,
                                  qniw,
                                  WP_IW_RX_BIND_MOD_PCE_INTERFACE,
                                  &rx_binding_cfg[0]);
    App_TerminateOnError(status,"WP_IwRxBindingModify",__LINE__);
    
}


WP_handle WPE_CreateNextRoundInterface(WP_U8 system_type)
{
    WP_handle pce_if_handle;
    WP_pce_if_params_iw_next_round pce_if_next_round_params = {0};

    if (SYSTEM_TYPE_VSI == system_type)
    {
        pce_if_next_round_params.parsing_ref_point = WP_PCE_PARSER_REF_POINT_MPLS_END;
        pce_if_next_round_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;        
        pce_if_next_round_params.filter_set_handle = filter_set_vpls;
    }
    else
    if (SYSTEM_TYPE_VRF == system_type)
    {
        pce_if_next_round_params.parsing_ref_point = WP_PCE_PARSER_REF_POINT_MPLS_END;
        pce_if_next_round_params.parser_start_type = WP_PCE_PARSER_START_TYPE_PURE_IPV4;        
        pce_if_next_round_params.filter_set_handle = filter_set_nni_vrf;
    }
    else
    if (SYSTEM_TYPE_VPWS == system_type)
    {
        pce_if_next_round_params.parsing_ref_point = WP_PCE_PARSER_REF_POINT_MPLS_END;
        pce_if_next_round_params.parser_start_type = WP_PCE_PARSER_START_TYPE_ETHERNET;        
        pce_if_next_round_params.filter_set_handle = filter_set_nni_vpws;
    }    
    else
    {
        return 0;
    }

    pce_if_next_round_params.mode = WP_PCE_IW_PORT_CONNECTION_ENABLED;
    pce_if_next_round_params.ip_header_validation = WP_DISABLE;
    pce_if_next_round_params.sub_port_vlan = WP_DISABLE;
    pce_if_next_round_params.collect_tpids = WP_DISABLE;    
    pce_if_next_round_params.parsing_ref_offset = 0;    
    
    pce_if_next_round_params.iw_port = WP_UNUSED;
    pce_if_next_round_params.characteristics = WP_PCE_IF_PRIORITY_MAPPING_PRESERVE | WP_PCE_IF_IW_PORT_PROPAGATION_ENABLE;
    
    pce_if_handle = WP_PceInterfaceCreate(WP_PCE_IF_TYPE_IW_NEXT_ROUND, &pce_if_next_round_params);
    App_TerminateOnError(pce_if_handle, "WP_PceInterfaceCreate NEXT ROUND", __LINE__);

    return pce_if_handle;

}


void WPE_CheckPceFilterStatsReset(void)
{
    WP_U32 i;
    WP_pce_filter_stat stat;
    WP_status status;

    memset(&stat, 0, sizeof(stat));

    for (i = FILTER_F1; i <= FILTER_F11; i++)
    {
        status = WP_PceFilterStatisticsReset (UNI_PCE_filter[i], &stat);
        App_TerminateOnError(status, "WP_PceFilterStatisticsReset", __LINE__);
    }

    for (i = FILTER_F1; i <= FILTER_F6; i++)
    {
        status = WP_PceFilterStatisticsReset (NNI_PCE_filter[i], &stat);
        App_TerminateOnError(status, "WP_PceFilterStatisticsReset", __LINE__);
    }    
}

void WPE_Reboot(void)
{
#if !defined(__linux__)
        /* Rebooting is specific to board and target.  The test
         * directory should not be concerned how this is done.
         * Instead, it calls a board-specific function. */
        if (WPI_REBOOT_ENABLE) 
        {
                printf("Test Rebooting winmon by WT_Reboot\nWinMon>\n");
                WPX_Reboot();
        }
        else
                printf("Not Rebooting with WT_Reboot\nWinMon>\n");
#endif
}

void WPE_Bridge_Quit(WP_U32 status)
{


    if (status)
    {
            WPE_Reboot();        
    }

    demo_running = FALSE;

}

void WPT_DisplayBridgingPortStats( WP_handle port_handle, WP_CHAR* type)
{
    WP_status status;
    WP_bport_stats bport_stats;
    WP_U32 temp2, temp1, index;
    memset(&bport_stats, 0, sizeof(bport_stats));
    status = WP_IwPortStatistics(port_handle, &bport_stats);
    App_TerminateOnError(status, "WP_IwPortStatistics",__LINE__);

    printf("---------------    handle 0x%08x    ---------------\n", port_handle);
    printf("-------------------------------------------------------\n");
    if (bport_stats.rx_valid_packets)
    {
        temp1 = bport_stats.rx_valid_packets ;
        temp2 = bport_stats.rx_valid_packets >> 32;
        printf("rx_valid_packets                       : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_vlan_acceptable_filter)
    {
        temp1 = bport_stats.discard_vlan_acceptable_filter ;
        temp2 = bport_stats.discard_vlan_acceptable_filter >> 32;
        printf("discard_vlan_acceptable_filter         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_ingress_filter)
    {
        temp1 = bport_stats.discard_ingress_filter ;
        temp2 = bport_stats.discard_ingress_filter >> 32;
        printf("discard_ingress_filter                 : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_bridge_classifier)
    {
        temp1 = bport_stats.discard_bridge_classifier ;
        temp2 = bport_stats.discard_bridge_classifier >> 32;
        printf("discard_bridge_classifier              : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_unk_macsa)
    {
        temp1 = bport_stats.discard_unk_macsa ;
        temp2 = bport_stats.discard_unk_macsa >> 32;
        printf("discard_unk_macsa                      : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.deny_mac_sa)
    {
        temp1 = bport_stats.deny_mac_sa ;
        temp2 = bport_stats.deny_mac_sa >> 32;
        printf("deny_mac_sa                            : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.deny_mac_da)
    {
        temp1 = bport_stats.deny_mac_da ;
        temp2 = bport_stats.deny_mac_da >> 32;
        printf("deny_mac_da                            : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.rx_bc_valid_packets)
    {
        temp1 = bport_stats.rx_bc_valid_packets ;
        temp2 = bport_stats.rx_bc_valid_packets >> 32;
        printf("rx_bc_valid_packets                    : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.rx_mc_valid_packets)
    {
        temp1 = bport_stats.rx_mc_valid_packets ;
        temp2 = bport_stats.rx_mc_valid_packets >> 32;
        printf("rx_mc_valid_packets                    : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.forwarded_uc_packets)
    {
        temp1 = bport_stats.forwarded_uc_packets ;
        temp2 = bport_stats.forwarded_uc_packets >> 32;
        printf("forwarded_uc_packets                   : %.8x%.8x\n", temp2, temp1);
    }    

    if (bport_stats.forwarded_bc_packets)
    {
        temp1 = bport_stats.forwarded_bc_packets ;
        temp2 = bport_stats.forwarded_bc_packets >> 32;
        printf("forwarded_bc_packets                   : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.forwarded_mc_packets)
    {
        temp1 = bport_stats.forwarded_mc_packets ;
        temp2 = bport_stats.forwarded_mc_packets >> 32;
        printf("forwarded_mc_packets                   : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.protocol_error)
    {
        temp1 = bport_stats.protocol_error ;
        temp2 = bport_stats.protocol_error >> 32;
        printf("protocol_error                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.discard_bridge_classifier)
    {
        temp1 = bport_stats.discard_bridge_classifier ;
        temp2 = bport_stats.discard_bridge_classifier >> 32;
        printf("discard_bridge_classifier              : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.res_mc_mac_deny)
    {
        temp1 = bport_stats.res_mc_mac_deny ;
        temp2 = bport_stats.res_mc_mac_deny >> 32;
        printf("res_mc_mac_deny                        : %.8x%.8x\n", temp2, temp1);
    }
    for (index = 0; index < WP_IW_DFC_MAX_NUM_OF_FILTERS; index++)
    {
        if (bport_stats.discard_dfc_filter[index])
        {
            temp1 = bport_stats.discard_dfc_filter[index] ;
            temp2 = bport_stats.discard_dfc_filter[index] >> 32;        
            printf("discard_dfc_filter[%02d]                 : %.8x%.8x\n", index, temp2, temp1);
        }        
    }
    
    if (bport_stats.deny_cfm_frames)
    {
        temp1 = bport_stats.deny_cfm_frames ;
        temp2 = 0;
        printf("deny_cfm_frames                        : %.8x%.8x\n", temp2, temp1);
    }
    
    if (bport_stats.bad_ip_version)
    {
        temp1 = bport_stats.bad_ip_version ;
        temp2 = bport_stats.bad_ip_version >> 32;
        printf("bad_ip_version                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.checksum_error)
    {
        temp1 = bport_stats.checksum_error ;
        temp2 = bport_stats.checksum_error >> 32;
        printf("checksum_error                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.ip_protocol_error)
    {
        temp1 = bport_stats.ip_protocol_error ;
        temp2 = bport_stats.ip_protocol_error >> 32;
        printf("ip_protocol_error                      : %.8x%.8x\n", temp2, temp1);
    }
    for (index = 0; index < WP_PCE_MAX_NUM_OF_FILTERS; index++)
    {
        if (bport_stats.discard_pce_filter[index])
        {
            temp1 = bport_stats.discard_pce_filter[index] ;
            temp2 = bport_stats.discard_pce_filter[index] >> 32;        
            printf("discard_pce_filter[%02d]                 : %.8x%.8x\n", index, temp2, temp1);
        }        
    }
    
    if (bport_stats.ingress_policer_non_conforming_packets)
    {
        temp1 = bport_stats.ingress_policer_non_conforming_packets ;
        temp2 = bport_stats.ingress_policer_non_conforming_packets >> 32;
        printf("ingress_policer_non_conforming_packets : %.8x%.8x\n", temp2, temp1);
    }   
    if (bport_stats.rx_discard_lpm)
    {
        temp1 = bport_stats.rx_discard_lpm ;
        temp2 = bport_stats.rx_discard_lpm >> 32;
        printf("rx_discard_lpm                         : %.8x%.8x\n", temp2, temp1);
    }
    if (bport_stats.iw_mirror_fbp_underrun)
    {
        temp1 = bport_stats.iw_mirror_fbp_underrun ;
        temp2 = bport_stats.iw_mirror_fbp_underrun >> 32;
        printf("iw_mirror_fbp_underrun                 : %.8x%.8x\n", temp2, temp1);
    }   
    if (bport_stats.iw_mirror_iwbg_fbp_underrun)
    {
        temp1 = bport_stats.iw_mirror_iwbg_fbp_underrun ;
        temp2 = bport_stats.iw_mirror_iwbg_fbp_underrun >> 32;
        printf("iw_mirror_iwbg_fbp_underrun            : %.8x%.8x\n", temp2, temp1);
    }


        printf("\n");
}

void WPE_DisplayBridgingPortStats(void)
{
    WP_U32 i, j;

    for (i = 0; i < NR_GBE; i++)
    {
        if (hw_port[i].bport_enet)
        {
            printf("\n               Port[%d] BridgePorts Statistics\n", i+1);
            WPT_DisplayBridgingPortStats(hw_port[i].bport_enet, "Port");
        }
        for (j = 0; j < MAX_SUBPORT_PER_PORT; j++)
        {
            if (hw_port[i].subport[j].valid && hw_port[i].subport[j].bport_enet)
            {
        printf("\n               ---SubPort[ %u - %u]\n", i+1, j+1);        
                WPT_DisplayBridgingPortStats(hw_port[i].subport[j].bport_enet, "SubPort");
            }
        }            

    }

    for (i = 0; i < NUM_OF_VPN_PORT; i++)
    {
        if (vpn_port[i].valid && vpn_port[i].bport)
        {
            printf("\n    VPN Port[%d] BridgePorts statistics\n", i+1);
            WPT_DisplayBridgingPortStats(vpn_port[i].bport, "VPNPort");                        
        }
    }

    for (i = 0; i < MAX_IW_SYSTEM; i++)
    {
        if (vpn_system[i].valid && vpn_system[i].flooding.dum_bport_handle)
        {
            printf("\n    System %d dummy BridgePorts statistics\n", i);
            WPT_DisplayBridgingPortStats(vpn_system[i].flooding.dum_bport_handle, "dummy bport");   
        }
    }

}

WP_U32 WPT_LocalDisplayDeviceStats(WP_handle enet_handle)
{
        WP_stats_enet stats_enet;
        WP_status status;
        WP_U32 temp1, temp2;
        if (!enet_handle)
        {
            return 0;
        }
        memset (&stats_enet,0,sizeof(WP_stats_enet));
    
        status = WP_DeviceStatistics(enet_handle, &stats_enet);
        App_TerminateOnError(status,"WP_DeviceStatistics",__LINE__);
    
        printf("---------------    handle 0x%08x    ---------------\n", enet_handle);
        printf("-------------------------------------------------------\n");
        if (stats_enet.txrx_frames_64)
        {
                temp1 = stats_enet.txrx_frames_64;
                temp2 = stats_enet.txrx_frames_64 >> 32;
                printf("txrx_frames_64:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_127)
        {
                temp1 = stats_enet.txrx_frames_127;
                temp2 = stats_enet.txrx_frames_127 >> 32;
                printf("txrx_frames_127:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_255)
        {
                temp1 = stats_enet.txrx_frames_255;
                temp2 = stats_enet.txrx_frames_255 >> 32;
                printf("txrx_frames_255:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_511)
        {
                temp1 = stats_enet.txrx_frames_511;
                temp2 = stats_enet.txrx_frames_511 >> 32;
                printf("txrx_frames_511:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_1023)
        {
                temp1 = stats_enet.txrx_frames_1023;
                temp2 = stats_enet.txrx_frames_1023 >> 32;
                printf("txrx_frames_1023:                   %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_1518)
        {
                temp1 = stats_enet.txrx_frames_1518;
                temp2 = stats_enet.txrx_frames_1518 >> 32;
                printf("txrx_frames_1518:                   %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.txrx_frames_1522)
        {
                temp1 = stats_enet.txrx_frames_1522;
                temp2 = stats_enet.txrx_frames_1522 >> 32;
                printf("txrx_frames_1522:                   %.8x%.8x\n", temp2,temp1);
        }

        if (stats_enet.rx_frames_64)
        {
                temp1 = stats_enet.rx_frames_64;
                temp2 = stats_enet.rx_frames_64 >> 32;
                printf("rx_frames_64:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_127)
        {
                temp1 = stats_enet.rx_frames_127;
                temp2 = stats_enet.rx_frames_127 >> 32;
                printf("rx_frames_127:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_255)
        {
                temp1 = stats_enet.rx_frames_255;
                temp2 = stats_enet.rx_frames_255 >> 32;
                printf("rx_frames_255:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_511)
        {
                temp1 = stats_enet.rx_frames_511;
                temp2 = stats_enet.rx_frames_511 >> 32;
                printf("rx_frames_511:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_1023)
        {
                temp1 = stats_enet.rx_frames_1023;
                temp2 = stats_enet.rx_frames_1023 >> 32;
                printf("rx_frames_1023:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_1518)
        {
                temp1 = stats_enet.rx_frames_1518;
                temp2 = stats_enet.rx_frames_1518 >> 32;
                printf("rx_frames_1518:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_frames_1522)
        {
                temp1 = stats_enet.rx_frames_1522;
                temp2 = stats_enet.rx_frames_1522 >> 32;
                printf("rx_frames_1522:                     %.8x%.8x\n", temp2,temp1);
        }        
        if (stats_enet.rx_bytes)
        {
                temp1 = stats_enet.rx_bytes;
                temp2 = stats_enet.rx_bytes >> 32;
                printf("rx_bytes:                           %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_packets)
        {
                temp1 = stats_enet.rx_packets;
                temp2 = stats_enet.rx_packets >> 32;
                printf("rx_packets:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_fcs)
        {
                temp1 = stats_enet.rx_err_fcs;
                temp2 = stats_enet.rx_err_fcs >> 32;
                printf("rx_err_fcs:                         %.8x%.8x\n", temp2,temp1);   
        }
        if (stats_enet.rx_multicast)
        {
                temp1 = stats_enet.rx_multicast;
                temp2 = stats_enet.rx_multicast >> 32;
                printf("rx_multicast:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_broadcast)
        {
                temp1 = stats_enet.rx_broadcast;
                temp2 = stats_enet.rx_broadcast >> 32;
                printf("rx_broadcast:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_mac_control)
        {
                temp1 = stats_enet.rx_mac_control;
                temp2 = stats_enet.rx_mac_control >> 32;
                printf("rx_mac_control:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_mac_pause)
        {
                temp1 = stats_enet.rx_mac_pause;
                temp2 = stats_enet.rx_mac_pause >> 32;
                printf("rx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_mac_unknown)
        {
                temp1 = stats_enet.rx_mac_unknown;
                temp2 = stats_enet.rx_mac_unknown >> 32;
                printf("rx_mac_unknown:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_alignment)
        {
                temp1 = stats_enet.rx_err_alignment;
                temp2 = stats_enet.rx_err_alignment >> 32;
                printf("rx_err_alignment:                   %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_length)
        {
                temp1 = stats_enet.rx_err_length;
                temp2 = stats_enet.rx_err_length >> 32;
                printf("rx_err_length:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_code)
        {
                temp1 = stats_enet.rx_err_code;
                temp2 = stats_enet.rx_err_code >> 32;
                printf("rx_err_code:                        %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_false_carrier)
        {
                temp1 = stats_enet.rx_false_carrier;
                temp2 = stats_enet.rx_false_carrier >> 32;
                printf("rx_false_carrier:                   %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_undersize)
        {
                temp1 = stats_enet.rx_undersize;
                temp2 = stats_enet.rx_undersize >> 32;
                printf("rx_undersize:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_oversize)
        {
                temp1 = stats_enet.rx_oversize;
                temp2 = stats_enet.rx_oversize >> 32;
                printf("rx_oversize:                        %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_fragments)
        {
                temp1 = stats_enet.rx_fragments;
                temp2 = stats_enet.rx_fragments >> 32;
                printf("rx_fragments:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_jabber)
        {
                temp1 = stats_enet.rx_jabber;
                temp2 = stats_enet.rx_jabber >> 32;
                printf("rx_jabber:                          %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_dropped)
        {
                temp1 = stats_enet.rx_dropped;
                temp2 = stats_enet.rx_dropped >> 32;
                printf("rx_dropped:                         %.8x%.8x\n", temp2,temp1);
        }
            
        if (stats_enet.tx_frames_64)
        {
                temp1 = stats_enet.tx_frames_64;
                temp2 = stats_enet.tx_frames_64 >> 32;
                printf("tx_frames_64:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_127)
        {
                temp1 = stats_enet.tx_frames_127;
                temp2 = stats_enet.tx_frames_127 >> 32;
                printf("tx_frames_127:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_255)
        {
                temp1 = stats_enet.tx_frames_255;
                temp2 = stats_enet.tx_frames_255 >> 32;
                printf("tx_frames_255:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_511)
        {
                temp1 = stats_enet.tx_frames_511;
                temp2 = stats_enet.tx_frames_511 >> 32;
                printf("tx_frames_511:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_1023)
        {
                temp1 = stats_enet.tx_frames_1023;
                temp2 = stats_enet.tx_frames_1023 >> 32;
                printf("tx_frames_1023:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_1518)
        {
                temp1 = stats_enet.tx_frames_1518;
                temp2 = stats_enet.tx_frames_1518 >> 32;
                printf("tx_frames_1518:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames_1522)
        {
                temp1 = stats_enet.tx_frames_1522;
                temp2 = stats_enet.tx_frames_1522 >> 32;
                printf("tx_frames_1522:                     %.8x%.8x\n", temp2,temp1);
        }            
        if (stats_enet.tx_bytes)
        {
                temp1 = stats_enet.tx_bytes;
                temp2 = stats_enet.tx_bytes >> 32;
                printf("tx_bytes:                           %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_packets)
        {
                temp1 = stats_enet.tx_packets;
                temp2 = stats_enet.tx_packets >> 32;
                printf("tx_packets:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_multicast)
        {
                temp1 = stats_enet.tx_multicast;
                temp2 = stats_enet.tx_multicast >> 32;
                printf("tx_multicast:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_broadcast)
        {
                temp1 = stats_enet.tx_broadcast;
                temp2 = stats_enet.tx_broadcast >> 32;
                printf("tx_broadcast:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_mac_pause)
        {
                temp1 = stats_enet.tx_mac_pause;
                temp2 = stats_enet.tx_mac_pause >> 32;
                printf("tx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_defer)
        {
                temp1 = stats_enet.tx_defer;
                temp2 = stats_enet.tx_defer >> 32;
                printf("tx_defer:                           %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_excess_defer)
        {
                temp1 = stats_enet.tx_excess_defer;
                temp2 = stats_enet.tx_excess_defer >> 32;
                printf("tx_excess_defer:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_single_collision)
        {
                temp1 = stats_enet.tx_single_collision;
                temp2 = stats_enet.tx_single_collision >> 32;
                printf("tx_single_collision:                %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_multi_collision)
        {
                temp1 = stats_enet.tx_multi_collision;
                temp2 = stats_enet.tx_multi_collision >> 32;
                printf("tx_multi_collision:                 %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_late_collision)
        {
                temp1 = stats_enet.tx_late_collision;
                temp2 = stats_enet.tx_late_collision >> 32;
                printf("tx_late_collision:                  %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_excess_collision)
        {
                temp1 = stats_enet.tx_excess_collision;
                temp2 = stats_enet.tx_excess_collision >> 32;
                printf("tx_excess_collision:                %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_no_collision)
        {
                temp1 = stats_enet.tx_no_collision;
                temp2 = stats_enet.tx_no_collision >> 32;
                printf("tx_no_collision:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_mac_pause_honored)
        {
                temp1 = stats_enet.tx_mac_pause_honored;
                temp2 = stats_enet.tx_mac_pause_honored >> 32;
                printf("tx_mac_pause_honored:               %.8x%.8x\n", temp2,temp1);
        }   
        if (stats_enet.tx_dropped)
        {
                temp1 = stats_enet.tx_dropped;
                temp2 = stats_enet.tx_dropped >> 32;
                printf("tx_dropped:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_jabber)
        {
                temp1 = stats_enet.tx_jabber;
                temp2 = stats_enet.tx_jabber >> 32;
                printf("tx_jabber:                          %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_err_fcs)
        {
                temp1 = stats_enet.tx_err_fcs;
                temp2 = stats_enet.tx_err_fcs >> 32;
                printf("tx_err_fcs:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_control)
        {
                temp1 = stats_enet.tx_control;
                temp2 = stats_enet.tx_control >> 32;
                printf("tx_control:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_oversize)
        {
                temp1 = stats_enet.tx_oversize;
                temp2 = stats_enet.tx_oversize >> 32;
                printf("tx_oversize:                        %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_undersize)
        {
                temp1 = stats_enet.tx_undersize;
                temp2 = stats_enet.tx_undersize >> 32;
                printf("tx_undersize:                       %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_fragments)
        {
                temp1 = stats_enet.tx_fragments;
                temp2 = stats_enet.tx_fragments >> 32;
                printf("tx_fragments:                       %.8x%.8x\n", temp2,temp1);
        }
            
        printf("DPS statistics:\n");
        if (stats_enet.rx_host_frames)
        {
                temp1 = stats_enet.rx_host_frames;
                temp2 = stats_enet.rx_host_frames >> 32;
                printf("rx_host_frames:                     %.8x%.8x\n", temp2,temp1);    
        }
        if (stats_enet.rx_iw_frames)
        {
                temp1 = stats_enet.rx_iw_frames;
                temp2 = stats_enet.rx_iw_frames >> 32;
                printf("rx_iw_frames:                       %.8x%.8x\n", temp2,temp1);    
        }
        if (stats_enet.rx_err_host_full)
        {
                temp1 = stats_enet.rx_err_host_full;
                temp2 = stats_enet.rx_err_host_full >> 32;
                printf("rx_err_host_full:                   %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_fbp_underrun)
        {
                temp1 = stats_enet.rx_err_fbp_underrun;
                temp2 = stats_enet.rx_err_fbp_underrun >> 32;
                printf("rx_err_fbp_underrun:                %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_nonvalid_mac)
        {
                temp1 = stats_enet.rx_err_nonvalid_mac;
                temp2 = stats_enet.rx_err_nonvalid_mac >> 32;
                printf("rx_err_mac_address:                 %.8x%.8x\n", temp2,temp1);
        }
        if ( stats_enet.rx_err_mru)
        {
                temp1 = stats_enet.rx_err_mru;
                temp2 = stats_enet.rx_err_mru >> 32;
                printf("rx_err_mru:                         %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_sw_state_machine)
        {
                temp1 = stats_enet.rx_err_sw_state_machine;
                temp2 = stats_enet.rx_err_sw_state_machine >> 32;
                printf("rx_err_sw_state:                    %.8x%.8x\n", temp2,temp1);        
        }
        if (stats_enet.rx_err_overrun)
        {
                temp1 = stats_enet.rx_err_overrun;
                temp2 = stats_enet.rx_err_overrun >> 32;
                printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_sdu)
        {
                temp1 = stats_enet.rx_err_sdu;
                temp2 = stats_enet.rx_err_sdu >> 32;
                printf("rx_err_maxsdu:                      %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_err_underrun)
        {
                temp1 = stats_enet.tx_err_underrun;
                temp2 = stats_enet.tx_err_underrun >> 32;
                printf("tx_err_underrun:                    %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.rx_err_overrun)
        {
                temp1 = stats_enet.rx_err_overrun;
                temp2 = stats_enet.rx_err_overrun >> 32;
                printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.tx_frames)
        {
                temp1 = stats_enet.tx_frames;
                temp2 = stats_enet.tx_frames >> 32;
                printf("tx_good_frames:                     %.8x%.8x\n", temp2,temp1);
        }
    
        printf("----------------------------------------------------\n");

        printf("Serial DPS IF statistics:\n");

        if (stats_enet.serial_dps_if_stats.tx_frames)
        {
                temp1 = stats_enet.serial_dps_if_stats.tx_frames;
                temp2 = stats_enet.serial_dps_if_stats.tx_frames >> 32;
                printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.serial_dps_if_stats.tx_bytes)
        {
                temp1 = stats_enet.serial_dps_if_stats.tx_bytes;
                temp2 = stats_enet.serial_dps_if_stats.tx_bytes >> 32;
                printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.serial_dps_if_stats.rx_frames)
        {
                temp1 = stats_enet.serial_dps_if_stats.rx_frames;
                temp2 = stats_enet.serial_dps_if_stats.rx_frames >> 32;
                printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.serial_dps_if_stats.rx_bytes)
        {
                temp1 = stats_enet.serial_dps_if_stats.rx_bytes;
                temp2 = stats_enet.serial_dps_if_stats.rx_bytes >> 32;
                printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.serial_dps_if_stats.rx_pce_denied_frames)
        {
                temp1 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames;
                temp2 = stats_enet.serial_dps_if_stats.rx_pce_denied_frames >> 32;
                printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
        }
        if (stats_enet.serial_dps_if_stats.rx_dropped_frames)
        {
                temp1 = stats_enet.serial_dps_if_stats.rx_dropped_frames;
                temp2 = stats_enet.serial_dps_if_stats.rx_dropped_frames >> 32;
                printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
        }
        printf("\n");

    
        return 0;
}


WP_U32 WPT_LocalDisplayXgiDeviceStats(WP_handle enet_handle)
{
        WP_stats_xgi enet_stats;
        WP_status status;
        WP_U32 temp1, temp2;
        if (!enet_handle)
        {
            return 0;
        }

        memset (&enet_stats,0,sizeof(WP_stats_xgi));

        status = WP_DeviceStatistics(enet_handle, &enet_stats);
        if (status)
        {
                printf("Error Reading Enet Statistics\n");
                exit(1);
        }

         printf("---------------    handle 0x%08x    ---------------\n", enet_handle);
         printf("-------------------------------------------------------\n");
         if (enet_stats.rx_frames_64)
         {
                 temp1 = enet_stats.rx_frames_64;
                 temp2 = enet_stats.rx_frames_64 >> 32;
                 printf("rx_frames_64:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_65_127)
         {
                 temp1 = enet_stats.rx_frames_65_127;
                 temp2 = enet_stats.rx_frames_65_127 >> 32;
                 printf("rx_frames_65_127:                   %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_128_255)
         {
                 temp1 = enet_stats.rx_frames_128_255;
                 temp2 = enet_stats.rx_frames_128_255 >> 32;
                 printf("rx_frames_128_255:                  %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_256_511)
         {
                 temp1 = enet_stats.rx_frames_256_511;
                 temp2 = enet_stats.rx_frames_256_511 >> 32;
                 printf("rx_frames_256_511:                  %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_512_1023)
         {
                 temp1 = enet_stats.rx_frames_512_1023;
                 temp2 = enet_stats.rx_frames_512_1023 >> 32;
                 printf("rx_frames_512_1023:                 %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_1024_1518)
         {
                 temp1 = enet_stats.rx_frames_1024_1518;
                 temp2 = enet_stats.rx_frames_1024_1518 >> 32;
                 printf("rx_frames_1024_1518:                %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_frames_1519_max)
         {
                 temp1 = enet_stats.rx_frames_1519_max;
                 temp2 = enet_stats.rx_frames_1519_max >> 32;
                 printf("rx_frames_1519_max:                 %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_bytes)
         {
                 temp1 = enet_stats.rx_bytes;
                 temp2 = enet_stats.rx_bytes >> 32;
                 printf("rx_bytes:                           %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_packets)
         {
                 temp1 = enet_stats.rx_packets;
                 temp2 = enet_stats.rx_packets >> 32;
                 printf("rx_packets:                         %.8x%.8x\n", temp2,temp1);
         }

         if (enet_stats.rx_multicast)
         {
                 temp1 = enet_stats.rx_multicast;
                 temp2 = enet_stats.rx_multicast >> 32;
                 printf("rx_multicast:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_broadcast)
         {
                 temp1 = enet_stats.rx_broadcast;
                 temp2 = enet_stats.rx_broadcast >> 32;
                 printf("rx_broadcast:                       %.8x%.8x\n", temp2,temp1);
         }
         
         if (enet_stats.rx_mac_pause)
         {
                 temp1 = enet_stats.rx_mac_pause;
                 temp2 = enet_stats.rx_mac_pause >> 32;
                 printf("rx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_undersize)
         {
                 temp1 = enet_stats.rx_undersize;
                 temp2 = enet_stats.rx_undersize >> 32;
                 printf("rx_undersize:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_oversize)
         {
                 temp1 = enet_stats.rx_oversize;
                 temp2 = enet_stats.rx_oversize >> 32;
                 printf("rx_oversize:                        %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_jabber)
         {
                 temp1 = enet_stats.rx_jabber;
                 temp2 = enet_stats.rx_jabber >> 32;
                 printf("rx_jabber:                          %.8x%.8x\n", temp2,temp1);
         }         
         if (enet_stats.rx_err_fcs)
         {
                 temp1 = enet_stats.rx_err_fcs;
                 temp2 = enet_stats.rx_err_fcs >> 32;
                 printf("rx_err_fcs:                         %.8x%.8x\n", temp2,temp1);   
         }         
         if (enet_stats.rx_err_length)
         {
                 temp1 = enet_stats.rx_err_length;
                 temp2 = enet_stats.rx_err_length >> 32;
                 printf("rx_err_length:                      %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.rx_err_code)
         {
                 temp1 = enet_stats.rx_err_code;
                 temp2 = enet_stats.rx_err_code >> 32;
                 printf("rx_err_code:                        %.8x%.8x\n", temp2,temp1);
         }
        
         if (enet_stats.tx_frames_64)
         {
                 temp1 = enet_stats.tx_frames_64;
                 temp2 = enet_stats.tx_frames_64 >> 32;
                 printf("tx_frames_64:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_65_127)
         {
                 temp1 = enet_stats.tx_frames_65_127;
                 temp2 = enet_stats.tx_frames_65_127 >> 32;
                 printf("tx_frames_65_127:                   %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_128_255)
         {
                 temp1 = enet_stats.tx_frames_128_255;
                 temp2 = enet_stats.tx_frames_128_255 >> 32;
                 printf("tx_frames_128_255:                  %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_256_511)
         {
                 temp1 = enet_stats.tx_frames_256_511;
                 temp2 = enet_stats.tx_frames_256_511 >> 32;
                 printf("tx_frames_256_511:                  %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_512_1023)
         {
                 temp1 = enet_stats.tx_frames_512_1023;
                 temp2 = enet_stats.tx_frames_512_1023 >> 32;
                 printf("tx_frames_512_1023:                 %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_1024_1518)
         {
                 temp1 = enet_stats.tx_frames_1024_1518;
                 temp2 = enet_stats.tx_frames_1024_1518 >> 32;
                 printf("tx_frames_1024_1518:                %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_frames_1519_max)
         {
                 temp1 = enet_stats.tx_frames_1519_max;
                 temp2 = enet_stats.tx_frames_1519_max >> 32;
                 printf("tx_frames_1519_max:                 %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_bytes)
         {
                 temp1 = enet_stats.tx_bytes;
                 temp2 = enet_stats.tx_bytes >> 32;
                 printf("tx_bytes:                           %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_packets)
         {
                 temp1 = enet_stats.tx_packets;
                 temp2 = enet_stats.tx_packets >> 32;
                 printf("tx_packets:                         %.8x%.8x\n", temp2,temp1);
         }

         if (enet_stats.tx_multicast)
         {
                 temp1 = enet_stats.tx_multicast;
                 temp2 = enet_stats.tx_multicast >> 32;
                 printf("tx_multicast:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_broadcast)
         {
                 temp1 = enet_stats.tx_broadcast;
                 temp2 = enet_stats.tx_broadcast >> 32;
                 printf("tx_broadcast:                       %.8x%.8x\n", temp2,temp1);
         }
         
         if (enet_stats.tx_mac_pause)
         {
                 temp1 = enet_stats.tx_mac_pause;
                 temp2 = enet_stats.tx_mac_pause >> 32;
                 printf("tx_mac_pause:                       %.8x%.8x\n", temp2,temp1);
         }
         if (enet_stats.tx_err_packets)
         {
                 temp1 = enet_stats.tx_err_packets;
                 temp2 = enet_stats.tx_err_packets >> 32;
                 printf("tx_err_packets:                     %.8x%.8x\n", temp2,temp1);
         }
            

        printf("DPS statistics:\n");
        if (enet_stats.rx_host_frames)
        {
                temp1 = enet_stats.rx_host_frames;
                temp2 = enet_stats.rx_host_frames >> 32;
                printf("rx_host_frames:                     %.8x%.8x\n", temp2,temp1);    
        }
        if (enet_stats.rx_iw_frames)
        {
                temp1 = enet_stats.rx_iw_frames;
                temp2 = enet_stats.rx_iw_frames >> 32;
                printf("rx_iw_frames:                       %.8x%.8x\n", temp2,temp1);    
        }
        if (enet_stats.rx_err_host_full)
        {
                temp1 = enet_stats.rx_err_host_full;
                temp2 = enet_stats.rx_err_host_full >> 32;
                printf("rx_err_host_full:                   %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.rx_err_fbp_underrun)
        {
                temp1 = enet_stats.rx_err_fbp_underrun;
                temp2 = enet_stats.rx_err_fbp_underrun >> 32;
                printf("rx_err_fbp_underrun:                %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.rx_err_nonvalid_mac)
        {
                temp1 = enet_stats.rx_err_nonvalid_mac;
                temp2 = enet_stats.rx_err_nonvalid_mac >> 32;
                printf("rx_err_mac_address:                 %.8x%.8x\n", temp2,temp1);
        }
        if ( enet_stats.rx_err_mru)
        {
                temp1 = enet_stats.rx_err_mru;
                temp2 = enet_stats.rx_err_mru >> 32;
                printf("rx_err_mru:                         %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.rx_err_sw_state_machine)
        {
                temp1 = enet_stats.rx_err_sw_state_machine;
                temp2 = enet_stats.rx_err_sw_state_machine >> 32;
                printf("rx_err_sw_state:                    %.8x%.8x\n", temp2,temp1);        
        }
        if (enet_stats.rx_err_overrun)
        {
                temp1 = enet_stats.rx_err_overrun;
                temp2 = enet_stats.rx_err_overrun >> 32;
                printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.rx_err_sdu)
        {
                temp1 = enet_stats.rx_err_sdu;
                temp2 = enet_stats.rx_err_sdu >> 32;
                printf("rx_err_maxsdu:                      %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.tx_err_underrun)
        {
                temp1 = enet_stats.tx_err_underrun;
                temp2 = enet_stats.tx_err_underrun >> 32;
                printf("tx_err_underrun:                    %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.rx_err_overrun)
        {
                temp1 = enet_stats.rx_err_overrun;
                temp2 = enet_stats.rx_err_overrun >> 32;
                printf("rx_err_overrun:                     %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.tx_frames)
        {
                temp1 = enet_stats.tx_frames;
                temp2 = enet_stats.tx_frames >> 32;
                printf("tx_good_frames:                     %.8x%.8x\n", temp2,temp1);
        }                        
        printf("----------------------------------------------------\n");

        printf("Serial DPS IF statistics:\n");

        if (enet_stats.serial_dps_if_stats.tx_frames)
        {
                temp1 = enet_stats.serial_dps_if_stats.tx_frames;
                temp2 = enet_stats.serial_dps_if_stats.tx_frames >> 32;
                printf("Serial DPS IF tx frames             %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.serial_dps_if_stats.tx_bytes)
        {
                temp1 = enet_stats.serial_dps_if_stats.tx_bytes;
                temp2 = enet_stats.serial_dps_if_stats.tx_bytes >> 32;
                printf("Serial DPS IF tx bytes              %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.serial_dps_if_stats.rx_frames)
        {
                temp1 = enet_stats.serial_dps_if_stats.rx_frames;
                temp2 = enet_stats.serial_dps_if_stats.rx_frames >> 32;
                printf("Serial DPS IF rx frames             %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.serial_dps_if_stats.rx_bytes)
        {
                temp1 = enet_stats.serial_dps_if_stats.rx_bytes;
                temp2 = enet_stats.serial_dps_if_stats.rx_bytes >> 32;
                printf("Serial DPS IF rx bytes              %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.serial_dps_if_stats.rx_pce_denied_frames)
        {
                temp1 = enet_stats.serial_dps_if_stats.rx_pce_denied_frames;
                temp2 = enet_stats.serial_dps_if_stats.rx_pce_denied_frames >> 32;
                printf("Serial DPS IF rx pce denied frames  %.8x%.8x\n", temp2,temp1);
        }
        if (enet_stats.serial_dps_if_stats.rx_dropped_frames)
        {
                temp1 = enet_stats.serial_dps_if_stats.rx_dropped_frames;
                temp2 = enet_stats.serial_dps_if_stats.rx_dropped_frames >> 32;
                printf("Serial DPS IF rx dropped frames     %.8x%.8x\n", temp2,temp1);
        }
        printf("\n");
        
        return 0;
}

void WPE_LocalDisplayDeviceStats(void)
{
    WP_U32 i;
    
    for (i = 0; i < NR_GBE; i++)
    {
        if (PORT_HW_XGI == hw_port[i].hw_type)
        {
            printf("\n               XGI[%d] Device Statistics\n", i);
            WPT_LocalDisplayXgiDeviceStats(hw_port[i].dev_enet);
        }
        else
        if (PORT_HW_SGMII == hw_port[i].hw_type)
        {
            printf("\n               Enet[%d] Device Statistics\n", i);
            WPT_LocalDisplayDeviceStats(hw_port[i].dev_enet);
        }
    }     
}


void WPT_GenericFlowStatistics( WP_handle agg)
{
        WP_iw_generic_flow_stats iw_flow_stats;

        WP_status status;
        WP_U32 temp1, temp2;
        if (!agg)
        {
            return;
        }

        memset(&iw_flow_stats,0,sizeof(iw_flow_stats));
        status = WP_IwFlowStatistics( agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
        App_TerminateOnError(status , "WP_IwFlowStatistics",__LINE__);

        printf ("Aggregation flow statistics of flow %d (0x%x)\n",
                (agg & 0x000000ff),agg);
        printf ("=================================================\n");

        if (iw_flow_stats.forward_packet)
        {
            temp1 =      (iw_flow_stats.forward_packet);    
            temp2 =      (iw_flow_stats.forward_packet)>> 32;
            printf ("forward_packet:                 %.8x%.8x\n", temp2, temp1);
        }       
       
        if (iw_flow_stats.mtu_drop_packets)
        {
            
            temp1 =      (iw_flow_stats.mtu_drop_packets);
            temp2 =      (iw_flow_stats.mtu_drop_packets)>> 32;
            printf ("mtu_drop_packets:               %.8x%.8x\n", temp2, temp1);
            
        }

          
        if (iw_flow_stats.tx_queue_drop_packets)
        {
            temp1 =      (iw_flow_stats.tx_queue_drop_packets);
            temp2 =      (iw_flow_stats.tx_queue_drop_packets)>> 32;
            printf ("tx_queue_drop_packets:          %.8x%.8x\n", temp2, temp1);
        }      

        if (iw_flow_stats.forwarded_bytes)
        {
            temp1 =      (iw_flow_stats.forwarded_bytes);
            temp2 =      (iw_flow_stats.forwarded_bytes)>> 32;
            
            printf ("forwarded_bytes:                %.8x%.8x\n", temp2, temp1);
        }          
        if (iw_flow_stats.denied_packets)
        {
            temp1 =      (iw_flow_stats.denied_packets);
            temp2 =      (iw_flow_stats.denied_packets)>> 32;
            
            printf ("denied_packets:                 %.8x%.8x\n", temp2, temp1);
        }         
         
              
        if (iw_flow_stats.policer_non_conforming_packets)
        {
            temp1 =      (iw_flow_stats.policer_non_conforming_packets);
            temp2 =      (iw_flow_stats.policer_non_conforming_packets)>> 32;
            
            printf ("policer_non_conforming_packets: %.8x%.8x\n", temp2, temp1);
        }

        if (iw_flow_stats.pecs_counter_1)
        {
            temp1 =      (iw_flow_stats.pecs_counter_1);
            temp2 =      (iw_flow_stats.pecs_counter_1)>> 32;
            
            printf ("pecs_counter_1:                 %.8x%.8x\n", temp2, temp1);
        }
        if (iw_flow_stats.pecs_counter_2)
        {
            temp1 =      (iw_flow_stats.pecs_counter_2);
            temp2 =      (iw_flow_stats.pecs_counter_2)>> 32;
            
            printf ("pecs_counter_2:                 %.8x%.8x\n", temp2, temp1);
        }
        if (iw_flow_stats.pecs_counter_3)
        {
            temp1 =      (iw_flow_stats.pecs_counter_3);
            temp2 =      (iw_flow_stats.pecs_counter_3)>> 32;
            
            printf ("pecs_counter_3:                 %.8x%.8x\n", temp2, temp1);
        } 
        if (iw_flow_stats.pecs_counter_4)
        {
            temp1 =      (iw_flow_stats.pecs_counter_4);
            temp2 =      (iw_flow_stats.pecs_counter_4)>> 32;
            
            printf ("pecs_counter_4:                 %.8x%.8x\n", temp2, temp1);
        }
        if (iw_flow_stats.pecs_counter_5)
        {
            temp1 =      (iw_flow_stats.pecs_counter_5);
            temp2 =      (iw_flow_stats.pecs_counter_5)>> 32;
            
            printf ("pecs_counter_5:                 %.8x%.8x\n", temp2, temp1);
        }
        if (iw_flow_stats.pecs_counter_6)
        {
            temp1 =      (iw_flow_stats.pecs_counter_6);
            temp2 =      (iw_flow_stats.pecs_counter_6)>> 32;
            
            printf ("pecs_counter_6:                 %.8x%.8x\n", temp2, temp1);
        }
        if (iw_flow_stats.group_filtered_packets)
        {
            temp1 =      (iw_flow_stats.group_filtered_packets);
            temp2 =      (iw_flow_stats.group_filtered_packets)>> 32;
            
            printf ("group_filtered_packets:         %.8x%.8x\n", temp2, temp1);
        }

}

void WPE_GenericFlowStatistics( void)
{
    WP_U32 i, j;
    for (i = 0; i < NR_GBE; i++)
    {
        if (hw_port[i].agg_enet)
        {
            printf("    Port[%d] flow statistics\n", hw_port[i].index);
            WPT_GenericFlowStatistics(hw_port[i].agg_enet);            

        }
        for (j = 0; j < MAX_SUBPORT_PER_PORT; j++)
        {
                if (hw_port[i].subport[j].agg_enet)
                {
            printf("\n               ---SubPort[%u - %u]\n", hw_port[i].index, hw_port[i].subport[j].index);        
                    WPT_GenericFlowStatistics(hw_port[i].subport[j].agg_enet);
                }
        
        }  

    }

    for (i = 0; i < NUM_OF_VPN_PORT; i++)
    {
        if (vpn_port[i].valid && vpn_port[i].flowagg)
        {
            printf("\n    VPN Port %d flow statistics\n", vpn_port[i].index);
            WPT_GenericFlowStatistics(vpn_port[i].flowagg);   
        }
    }

    for (i = 0; i < MAX_IW_SYSTEM; i++)
    {
        if (vpn_system[i].valid && vpn_system[i].flooding.agg_handle)
        {
            printf("\n    System %d flooding flow statistics\n", i);
            WPT_GenericFlowStatistics(vpn_system[i].flooding.agg_handle);   
        }
    }

}


void WPT_IwBridgingPortStatsReset(WP_handle port_handle)
{
        WP_bport_stats bport_stats;
        WP_status status;

        if (!port_handle)
                return;
   
        status = WP_IwPortStatisticsReset(port_handle, &bport_stats);
        App_TerminateOnError(status , "WP_IwPortStatisticsReset()",__LINE__);
}

void WPE_IwBridgingPortStatsReset(void)
{
    
    WP_U32 i, j;

    for (i = 0; i < NR_GBE; i++)
    {
        if (hw_port[i].bport_enet)
        {
            WPT_IwBridgingPortStatsReset(hw_port[i].bport_enet);         
        }
        for (j = 0; j < MAX_SUBPORT_PER_PORT; j++)
        {
            if (hw_port[i].subport[j].valid && hw_port[i].subport[j].bport_enet)
            {
                WPT_IwBridgingPortStatsReset(hw_port[i].subport[j].bport_enet);
            }
        }           
    }

    for (i = 0; i < NUM_OF_VPN_PORT; i++)
    {
        if (vpn_port[i].valid && vpn_port[i].bport)
        {
            WPT_IwBridgingPortStatsReset(vpn_port[i].bport);                        
        }
    }

    for (i = 0; i < MAX_IW_SYSTEM; i++)
    {
        if (vpn_system[i].valid && vpn_system[i].flooding.dum_bport_handle)
        {
            WPT_IwBridgingPortStatsReset(vpn_system[i].flooding.dum_bport_handle);   
        }
    }

    printf("Done!\n");


}

void WPT_LocalDeviceStatsReset(WP_handle enet_handle)
{
        WP_stats_enet enet_stats;
        WP_status status;

        status = WP_DeviceStatisticsReset(enet_handle, &enet_stats);
        App_TerminateOnError(status, "WP_DeviceStatisticsReset()",__LINE__);
}

void WPE_LocalDeviceStatsReset(void)
{
    WP_U32 i;
    
    for (i = 0; i < NR_GBE; i++)
    {
        if (hw_port[i].dev_enet)
        {
            WPT_LocalDeviceStatsReset(hw_port[i].dev_enet);
        }            
    }

    printf("Done!\n");

}


void WPT_GenericFlowStatisticsReset(WP_handle agg)
{
        WP_iw_flow_stats iw_flow_stats;
        WP_status status;

        status = WP_IwFlowStatisticsReset(agg, WP_IW_FLOW_STAT_GENERIC, &iw_flow_stats);
        App_TerminateOnError(status , "WP_IwFlowStatisticsReset()",__LINE__);
}

void WPE_GenericFlowStatisticsReset(void)
{
    WP_U32 i, j;
    for (i = 0; i < NR_GBE; i++)
    {
        if (PORT_HW_NA == hw_port[i].hw_type)
        {
            continue;
        }
        if (hw_port[i].agg_enet)
        {
            WPT_GenericFlowStatisticsReset(hw_port[i].agg_enet);            
        }
        for (j = 0; j < MAX_SUBPORT_PER_PORT; j++)
        {
            if (hw_port[i].subport[j].valid && hw_port[i].subport[j].agg_enet)
            {
                WPT_GenericFlowStatisticsReset(hw_port[i].subport[j].agg_enet);
            }
        }  

    }

    for (i = 0; i < NUM_OF_VPN_PORT; i++)
    {
        if (vpn_port[i].valid && vpn_port[i].flowagg)
        {
            WPT_GenericFlowStatisticsReset(vpn_port[i].flowagg);   
        }
    }

    for (i = 0; i < MAX_IW_SYSTEM; i++)
    {
        if (vpn_system[i].valid && vpn_system[i].flooding.agg_handle)
        {
            WPT_GenericFlowStatisticsReset(vpn_system[i].flooding.agg_handle);   
        }
    }

    printf("Done!\n");

}

void WPT_PCEStatistics(WP_handle handle)
{
    WP_status status;
    WP_pce_filter_stat pce_filter_stats;
    WP_U32 temp2, temp1;
    memset(&pce_filter_stats, 0, sizeof(pce_filter_stats));
    status = WP_PceFilterStatistics(handle, &pce_filter_stats);
    App_TerminateOnError(status, "WP_PceFilterStatistics",__LINE__);

    printf("---------------    handle 0x%08x    ---------------\n", handle);
    printf("-------------------------------------------------------\n");
    if (pce_filter_stats.denied_pkts)
    {
        temp1 = pce_filter_stats.denied_pkts ;
        temp2 = pce_filter_stats.denied_pkts >> 32;
        printf("denied_pkts                           : %.8x%.8x\n", temp2, temp1);
    }    
    if (pce_filter_stats.matched_pkts)
    {
        temp1 = pce_filter_stats.matched_pkts ;
        temp2 = pce_filter_stats.matched_pkts >> 32;
        printf("matched_pkts                          : %.8x%.8x\n", temp2, temp1);
    }  
    if (pce_filter_stats.no_matched_pkts)
    {
        temp1 = pce_filter_stats.no_matched_pkts ;
        temp2 = pce_filter_stats.no_matched_pkts >> 32;
        printf("no_matched_pkts                       : %.8x%.8x\n", temp2, temp1);
    }  
    printf("\n");

}

void WPT_PCEGlobalStatistics(void)
{
    WP_status status;
    WP_pce_global_stat pce_filter_stats;
    WP_U32 temp2, temp1;
    memset(&pce_filter_stats, 0, sizeof(pce_filter_stats));
    status = WP_PceGlobalStatistics(0, &pce_filter_stats);
    App_TerminateOnError(status, "WP_PceFilterStatistics",__LINE__);
    if (pce_filter_stats.parser_header_access_errors)
    {
        temp1 = pce_filter_stats.parser_header_access_errors ;
        temp2 = pce_filter_stats.parser_header_access_errors >> 32;
        printf("parser_header_access_errors           : %.8x%.8x\n", temp2, temp1);
        
    }
    if (pce_filter_stats.parser_timeout_err)
    {
        temp1 = pce_filter_stats.parser_timeout_err ;
        temp2 = pce_filter_stats.parser_timeout_err >> 32;
        printf("parser_timeout_err                    : %.8x%.8x\n", temp2, temp1);
        
    }    
    if (pce_filter_stats.classifier_timeout_err)
    {
        temp1 = pce_filter_stats.classifier_timeout_err ;
        temp2 = pce_filter_stats.classifier_timeout_err >> 32;
        printf("classifier_timeout_err                : %.8x%.8x\n", temp2, temp1);
        
    }
    if (pce_filter_stats.parser_accept)
    {
        temp1 = pce_filter_stats.parser_accept ;
        temp2 = pce_filter_stats.parser_accept >> 32;
        printf("parser_accept                         : %.8x%.8x\n", temp2, temp1);
    }

}
void WPE_PCEStatistics(void)
{
    WP_U32 i;
    printf("PCEGlobalStatistics\r\n");
    WPT_PCEGlobalStatistics();
    printf("\r\n");

    for (i = 0; i <= FILTER_F11; i++)
    {
        printf("UNI Pce Filter [%d] Statistics\n", i);

        WPT_PCEStatistics (UNI_PCE_filter[i]);
    }

    for (i = FILTER_F1; i <= FILTER_F6; i++)
    {
        printf("NNI Pce Filter [%d] Statistics\n", i);
        WPT_PCEStatistics (NNI_PCE_filter[i]);
    } 

}

void WPE_PCEStatisticsReset(void)
{
    WP_U32 i;
    WP_status status;

    WP_pce_global_stat global_stats;
    WP_pce_filter_stat filter_stats;
    status =  WP_PceGlobalStatisticsReset (0, &global_stats);
    App_TerminateOnError(status, "WP_PceGlobalStatisticsReset", __LINE__);


    for (i = 0; i <= FILTER_F11; i++)
    {
        status = WP_PceFilterStatisticsReset (UNI_PCE_filter[i], &filter_stats);
        App_TerminateOnError(status, "WP_PolicerStatistics", __LINE__);
    }

    for (i = FILTER_F1; i <= FILTER_F6; i++)
    {
        status = WP_PceFilterStatisticsReset (NNI_PCE_filter[i], &filter_stats);
        App_TerminateOnError(status, "WP_PolicerStatistics", __LINE__);
    } 

    printf("\n               Done!\n");
}


void WPT_PolicerStatistics(WP_handle handle)
{
    WP_status status;
    WP_stats_policer_v2 stats;
    WP_U32 temp2, temp1;
    memset(&stats, 0, sizeof(stats));
    status = WP_PolicerStatistics(handle, &stats);
    App_TerminateOnError(status, "WP_PolicerStatistics", __LINE__);

    printf("---------------    handle 0x%08x    ---------------\n", handle);
    printf("-------------------------------------------------------\n");
    if (stats.non_conforming_cbs)
    {
        temp1 = stats.non_conforming_cbs ;
        temp2 = stats.non_conforming_cbs >> 32;
        printf("non_conforming_cbs                    : %.8x%.8x\n", temp2, temp1);
    }    
    if (stats.non_conforming_ebs)
    {
        temp1 = stats.non_conforming_ebs ;
        temp2 = stats.non_conforming_ebs >> 32;
        printf("non_conforming_ebs                    : %.8x%.8x\n", temp2, temp1);
    }  

    printf("\n");

}

void WPE_PolicerStatistics(void)
{
    WP_U32 i;

    for (i = 0; i < NUM_OF_POLICER; i++)
    {
        if (ing_policer[i].handle)
        {
            printf("    VPN port [%d] Policer Statistics\n", ing_policer[i].vpnport);

            WPT_PolicerStatistics(ing_policer[i].handle);
        }
    }
}

void WPE_PolicerStatisticsReset(void)
{
    WP_U32 i;
    WP_status status;

    for (i = 0; i < NUM_OF_POLICER; i++)
    {
        if (ing_policer[i].handle)
        {
            status = WP_PolicerStatisticsReset(ing_policer[i].handle);
            App_TerminateOnError(status, "WP_PolicerStatisticsReset", __LINE__);
        }
    }

    printf("done!\n");
}


void WPT_DisplayGroupStats(WP_handle group)
{
    WP_U32 temp2, temp1;
   WP_iw_mc_class_stats iw_mc_class_stats;
   WP_status status;

   memset (&iw_mc_class_stats, 0, sizeof(iw_mc_class_stats));
   status=WP_IwMcStatistics(group, 0, &iw_mc_class_stats);
   App_TerminateOnError(status, "WP_IwMcStatistics()",__LINE__);
   
   printf ("\nMC group statistics of group %d (0x%x)\n",(group & 0x000000ff), group);
   printf ("==========================================================\n");
    if (iw_mc_class_stats.buffers_counter)
    {
        temp1 = iw_mc_class_stats.buffers_counter ;
        temp2 = iw_mc_class_stats.buffers_counter >> 32;
        printf("Buffer Counter                          : %.8x%.8x\n", temp2, temp1);
    }   
    if (iw_mc_class_stats.threshold_dropped_frames)
    {
        temp1 = iw_mc_class_stats.threshold_dropped_frames ;
        temp2 = iw_mc_class_stats.threshold_dropped_frames >> 32;
        printf("Threshold Dropped Frames                : %.8x%.8x\n", temp2, temp1);
    } 
    if (iw_mc_class_stats.forwarded_frames)
    {
        temp1 = iw_mc_class_stats.forwarded_frames ;
        temp2 = iw_mc_class_stats.forwarded_frames >> 32;
        printf("Forwarded Frames                        : %.8x%.8x\n", temp2, temp1);
    } 

   printf ("\n");
   
   return ;
}

void WPT_ResetGroupStats(WP_handle group)
{

   WP_iw_mc_class_stats iw_mc_class_stats;
   WP_status status;

   memset (&iw_mc_class_stats, 0, sizeof(iw_mc_class_stats));
   status=WP_IwMcStatisticsReset(group, 0, &iw_mc_class_stats);
   App_TerminateOnError(status, "WP_IwMcStatisticsReset()",__LINE__);
}

void WPT_DisplayMcMemberEnhancedStats(WP_handle group, WP_handle member)
{
    WP_status status = WP_OK;
    WP_iw_mc_member_enhanced_stats iw_mc_member_enhanced_stats = {0};

    memset(&iw_mc_member_enhanced_stats, 0, sizeof(iw_mc_member_enhanced_stats));
    status = WP_IwMcMemberStatistics(group, member, &iw_mc_member_enhanced_stats);
    App_TerminateOnError(status, "WP_IwMcMemberStatistics()",__LINE__);                                           

   printf("\n MC Member (0x%x) Enhanced statistics \n", member);
   printf("==========================================================\n");
   if (iw_mc_member_enhanced_stats.forwarded_frames)
   {
        printf("\nforwarded_frames:               ");
        PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.forwarded_frames);
   }
   if (iw_mc_member_enhanced_stats.forwarded_bytes)
   {
       printf("\nforwarded_bytes:                ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.forwarded_bytes);
   
   }

   if (iw_mc_member_enhanced_stats.buffers_counter)
   {
       printf("\nbuffers_counter:                ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.buffers_counter);    
   }
   if (iw_mc_member_enhanced_stats.threshold_dropped_frames)
   {
       printf("\nthreshold_dropped_frames:       ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.threshold_dropped_frames);    
   }
   if (iw_mc_member_enhanced_stats.class_threshold_dropped_frames)
   {
        
       printf("\nclass_threshold_dropped_frames: ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.class_threshold_dropped_frames);
   }
   if (iw_mc_member_enhanced_stats.fbp_drop_packets)
   {
       printf("\nfbp_drop_packets:               ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.fbp_drop_packets);
    
   }
   if (iw_mc_member_enhanced_stats.mtu_drop_packets)
   {
       printf("\nmtu_drop_packets:               ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.mtu_drop_packets);
   }
   if (iw_mc_member_enhanced_stats.ttl_drop_packets)
   {
       printf("\nttl_drop_packets:               ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.ttl_drop_packets);
   }
   if (iw_mc_member_enhanced_stats.tx_queue_drop_packets)
   {
       printf("\ntx_queue_drop_packets:          ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.tx_queue_drop_packets);
   }
   if (iw_mc_member_enhanced_stats.mpls_drop)
   {
       printf("\nmpls_drop:                      ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.mpls_drop);
   }
   if (iw_mc_member_enhanced_stats.denied_packets)
   {       
       printf("\ndenied_packets:                 ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.denied_packets);
   }
   if (iw_mc_member_enhanced_stats.group_filtered_packets)
   {
       printf("\ngroup_filtered_packets:         ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.group_filtered_packets);
   }
   if (iw_mc_member_enhanced_stats.fbp_underrun)
   {
       printf("\nfbp_underrun:                   ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.fbp_underrun);
   }
   if (iw_mc_member_enhanced_stats.src_ip_filter_dropped_frames)
   {
       printf("\nsrc_ip_filter_dropped_frames:   ");
       PRINT_STAT_FIELD(iw_mc_member_enhanced_stats.src_ip_filter_dropped_frames);
   }

   printf("\n\n");
}

void WPT_ResetMcMemberEnhancedStats(WP_handle group, WP_handle member)
{
    WP_status status = WP_OK;
    WP_iw_mc_member_enhanced_stats iw_mc_member_enhanced_stats = {0};

    memset(&iw_mc_member_enhanced_stats, 0, sizeof(iw_mc_member_enhanced_stats));
    status = WP_IwMcMemberStatisticsReset(group, member, &iw_mc_member_enhanced_stats);
    App_TerminateOnError(status, "WP_IwMcMemberStatisticsReset()",__LINE__);
}    

void WPE_FloodingStatistics(void)
{
    WP_U32 ii, jj;


    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (vpn_system[ii].valid && (SYSTEM_TYPE_VSI == vpn_system[ii].type) )
        {
            if (vpn_system[ii].flooding.group_handle)
            {
                printf("\nSystem ID %d Flooding Statistics\r\n", vpn_system[ii].index);
                printf("==========================================================\n");
                
                //WPT_DisplayGroupStats(vpn_system[ii].flooding.group_handle);
            }
            
            for (jj = 0; jj < MAX_PORT_PER_SYSTEM; jj++)
            {
                if (vpn_system[ii].flooding.member_handle[jj])
                {
                    WPT_DisplayMcMemberEnhancedStats(vpn_system[ii].flooding.group_handle, vpn_system[ii].flooding.member_handle[jj]);
                }                 
            }
              
        }
    }    
}

void WPE_FloodingStatisticsReset(void)
{
    WP_U32 ii, jj;

    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (vpn_system[ii].valid && (SYSTEM_TYPE_VSI == vpn_system[ii].type) )
        {
            if (vpn_system[ii].flooding.group_handle)
            {                
                //WPT_ResetGroupStats(vpn_system[ii].flooding.group_handle);
            }
            
            for (jj = 0; jj < MAX_PORT_PER_SYSTEM; jj++)
            {
                if (vpn_system[ii].flooding.member_handle[jj])
                {
                    WPT_ResetMcMemberEnhancedStats(vpn_system[ii].flooding.group_handle, vpn_system[ii].flooding.member_handle[jj]);
                }                 
            }              
        }
    }    

    printf("\nFlooding Member Statistics Reset done!\r\n");

}

/****************************************fdb function**************************/

/*****mac operate**********/


/**create node*/
Node * WPE_CreateFDBList(unsigned char *mac, WP_U16 vlan,WP_U16 port, WP_handle rule_handle)
{
        Node *temp =  NULL;
        temp=(Node*)malloc(sizeof(Node));
        if(temp == NULL)
        {
                printf("malloc failed!\n");
                return NULL;
        }
        memcpy(temp->mac, mac, 6);
        temp->static_mac = 0;
        temp->vpnport = port;
        temp->pce_rule = rule_handle;
        temp->next=NULL;       
         
        return temp;
}

/*insert node*/

Node *WPE_InsertFDBNode(Node *Fdbhead, unsigned char *mac, WP_U16 vlan, WP_U16 port, WP_handle rule_handle)
{
    Node *current;    
    Node *prev=Fdbhead;            
    Node *temp = NULL;
    if( NULL == Fdbhead)
            return Fdbhead;
            
    current=Fdbhead->next; 

    while(current)                
    {
        prev=current;
        current=current->next;    
    }

    temp=(Node*) malloc( sizeof(Node) );
    if(temp == NULL)
    {
            printf("malloc failed!\n");
            return NULL;
    }
    memcpy(temp->mac, mac, 6);
    temp->vpnport = port;
    temp->static_mac = 0;
    temp->pce_rule = rule_handle;
    temp->next=NULL;
    prev->next=temp;            

    return Fdbhead;
}


void WPE_AddFDBNode(WP_pce_rule_forwarding *rule)
{
    Node * ret = NULL;
    WP_U16 i, port = 0xffff;
    unsigned char *mac = rule->rule_fields[1].value.mac_addr;
    WT_VsiSystemInfo* pSystem = WPE_GetSystemByHandle(rule->rule_fields[0].value.iw_system_handle);    
    WP_handle handle = rule->match_result[0].param.iw_port.iw_port_handle;

    if ((NULL == pSystem) || (!pSystem->learn_en))
    {
        return;
    }

    for (i = 0; i < NUM_OF_VPN_PORT; i++)
    {
        if (vpn_port[i].bport == handle)
        {
            port = vpn_port[i].index;
            break;
        }
    }
    
    if (i >= NUM_OF_VPN_PORT)
    {
        printf("Can't find iw port 0x%x\n", handle);
        return;
    }

    /* Enable aging and set weight */
    if (pSystem->aging_en)
    {
        rule->match_result[1].param.aging.mode = WP_PCE_AGING_ENABLE;
        rule->match_result[1].param.aging.weight = pSystem->aging_weight;
    }
    else
    {
        rule->match_result[1].param.aging.mode = WP_PCE_AGING_DISABLE;
    }

    if (WP_PCE_RESULT_LAST == rule->match_result[2].result_type)
    {
        rule->match_result[2].result_type = WP_PCE_RESULT_FLOW_AGG;
        rule->match_result[2].param.flow_agg.flow_aggregation = vpn_port[i].flowagg;
        
        rule->match_result[3].result_type = WP_PCE_RESULT_LAST;
    }


    handle = WP_PceRuleCreate(WP_WINPATH(DEFAULT_WPID),
                                       WP_PCE_RULE_FORWARDING,
                                       rule);
    if (WP_ERROR(handle) == WP_ERR_PCE_RULE_ALREADY_EXISTS)
    {
        return;
    }
    else
    {
        App_TerminateOnError(handle, "WP_PceRuleCreate", __LINE__);
    }

    if(pSystem->Fdbhead == NULL)
    {
        pSystem->Fdbhead = WPE_CreateFDBList(mac, 0, port, handle);
        ret = pSystem->Fdbhead;
    }
    else
    {
        ret = WPE_InsertFDBNode(pSystem->Fdbhead, mac, 0, port, handle);
    }

    if (ret)
    {
        vpn_port[i].mac_num++;
    }
    else
    {
        WP_status status = WP_PceRuleDelete(handle);
        App_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);        
    }

}

Node *WPE_DeleteFDBNodeByPort(WT_VsiSystemInfo* pSystem, WP_U16 port)
{
        WP_status status;
        Node *prev,*current,*temp;

        if((NULL == pSystem) || (NULL == pSystem->Fdbhead))
                return NULL;

        current=pSystem->Fdbhead;
        prev = pSystem->Fdbhead;

        while(current)
        {
                if((current->vpnport == port))
                {
                        status = WP_PceRuleDisable(current->pce_rule);
                        App_TerminateOnError(status, "WP_PceRuleDisable", __LINE__);                
                        status = WP_PceRuleDelete(current->pce_rule);
                        App_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);
                        if (current == pSystem->Fdbhead) 
                        {
                                pSystem->Fdbhead = current->next;
                                prev = pSystem->Fdbhead;
                        }
                        else 
                        {
                                prev->next = current->next;
                                
                        }
                        temp = current;
                        current=current->next;
                        free(temp);
                        if ((port <= NUM_OF_VPN_PORT) && (port > 0))
                        {
                            vpn_port[port-1].mac_num--;
                        }           			   
                        continue;
                }
                else 
                {
                        prev = current;
                        current=current->next;
                }
        }

        return pSystem->Fdbhead;

}


/*delete node by handle*/
void WPE_DeleteFDBNodeByHandle(WP_handle rule_handle)
{
    WP_U16 ii;
    Node *prev,*current;
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (NULL == vpn_system[ii].Fdbhead)
        {
            continue;
        }

        prev=vpn_system[ii].Fdbhead;
        current=vpn_system[ii].Fdbhead->next;

        if(prev->pce_rule == rule_handle)
        {
                free(vpn_system[ii].Fdbhead);
                vpn_system[ii].Fdbhead = current;
                return ;
        }
    
        while(current)
        {
                if(current->pce_rule == rule_handle)        
                {
                
                        prev->next=current->next;    
                        free(current);            
                        return ;
                }
                prev=current;
                current=current->next;            
        }
    }
    printf("Can't find pce rule 0x%x\n", rule_handle);
}


Node *WPE_GetFDBNodeByHandle(Node *Fdbhead, WP_handle rule_handle)
{
    Node *current = Fdbhead;

    while(current)
    {
        if(current->pce_rule == rule_handle)        
        {          
            return current;
        }
        current = current->next;
    }

    return NULL;    
}

WT_VsiSystemInfo* WPE_GetSystemByFdbHandle(WP_handle rule_handle)
{
    WP_U16 ii;
    Node *current;
    for (ii = 0; ii < MAX_IW_SYSTEM; ii++)
    {
        if (0 == vpn_system[ii].valid)
        {
            continue;
        }
        
        if (NULL == vpn_system[ii].Fdbhead)
        {
            continue;
        }

        current = vpn_system[ii].Fdbhead;

        while(current)
        {
            if(current->pce_rule == rule_handle)        
            {            
                return &vpn_system[ii];
            }
            current=current->next;            
        }
    }

    return NULL;
}


void WPE_AddFDBEntry(WP_pce_rule_forwarding *rule)
{
        WPE_AddFDBNode(rule);
        //WPE_PrintFDBList();
        return;

}

void WPE_DelFDBEntry(WP_handle rule_handle)
{
        WPE_DeleteFDBNodeByHandle(rule_handle);
        return;
}

void WPE_DelSystemFDBEntry(WT_VsiSystemInfo* pSystem, WP_handle rule_handle)
{
    WP_U8 port ;
    WP_status status;
    Node *prev,*current,*temp;
    
    if ((NULL == pSystem) || (NULL == pSystem->Fdbhead))
    {
        return;
    }
    
    current=pSystem->Fdbhead;
    prev = pSystem->Fdbhead;
    
    while(current)
    {
            if((current->pce_rule == rule_handle))
            {
                    status = WP_PceRuleDisable(current->pce_rule);
                    App_TerminateOnError(status, "WP_PceRuleDisable", __LINE__);                
                    status = WP_PceRuleDelete(current->pce_rule);
                    App_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);
                    if (current == pSystem->Fdbhead) 
                    {
                            pSystem->Fdbhead = current->next;
                            prev = pSystem->Fdbhead;
                    }
                    else 
                    {
                            prev->next = current->next;
                    }
                    port = current->vpnport;

                    temp = current;
                    current=current->next;
                    free(temp);
                    if ((port <= NUM_OF_VPN_PORT) && (port > 0))
                    {
                        vpn_port[port-1].mac_num--;
                    }           
                    return;
            }
            else 
            {
                    prev = current;
                    current=current->next;
            }
    }

}


int WPE_DelLearntMac(WP_U8 vpnid, WP_U8 mac[6])
{
    WP_U8 port ;
    WP_status status;
    Node *prev,*current,*temp;
    WT_VsiSystemInfo* pVpn = NULL;
    
    if ((vpnid < 1) || (vpnid > MAX_IW_SYSTEM))
    {
        return -1;
    }
    
    pVpn = vpn_system + vpnid - 1;
    if ((!pVpn->valid) || (SYSTEM_TYPE_VSI != pVpn->type))
    {
        return -2;
    }
    
    current=pVpn->Fdbhead;
    prev = pVpn->Fdbhead;
    
    while(current)
    {
            if(0 == memcmp(current->mac, mac, 6))
            {
                    status = WP_PceRuleDisable(current->pce_rule);
                    App_TerminateOnError(status, "WP_PceRuleDisable", __LINE__);                
                    status = WP_PceRuleDelete(current->pce_rule);
                    App_TerminateOnError(status, "WP_PceRuleDelete",__LINE__);
                    if (current == pVpn->Fdbhead) 
                    {
                            pVpn->Fdbhead = current->next;
                            prev = pVpn->Fdbhead;
                    }
                    else 
                    {
                            prev->next = current->next;
                    }
                    port = current->vpnport;

                    temp = current;
                    current=current->next;
                    free(temp);
                    if ((port <= NUM_OF_VPN_PORT) && (port > 0))
                    {
                        vpn_port[port-1].mac_num--;
                    }           
            }
            else 
            {
                    prev = current;
                    current=current->next;
            }
    }

    return 0;
}


/********************************************************************************/


WP_U32 WT_TimeRead(void)
{
    return WP_TimeRead();
}

WP_U32 WT_TimeDelta(WP_U32 later,WP_U32 earlier)
{
    return WP_TimeDelta(later,earlier);
}


#ifndef __linux__

#define AF_INET         4
#define AF_INET6        6

typedef unsigned int    __u32;
typedef __u32           __be32;
typedef __u32           uint32;

struct in_addr {
        __be32 s_addr;
};

#define u_int          __u32

static int      inet_pton4(const char *src, u_char *dst);  
static int      inet_pton6(const char *src, u_char *dst);  
  
/* int 
 * inet_pton(af, src, dst) 
 *      convert from presentation format (which usually means ASCII printable) 
 *      to network format (which is usually some kind of binary format). 
 * return: 
 *      1 if the address was valid for the specified address family 
 *      0 if the address wasn't valid (`dst' is untouched in this case) 
 *      -1 if some other error occurred (`dst' is untouched in this case, too) 
 * author: 
 *      Paul Vixie, 1996. 
 */  
int  
inet_pton(int af, const char *src, void *dst)  
{  
        switch (af) {  
        case AF_INET:  
                return (inet_pton4(src, dst));  
        case AF_INET6:  
                return (inet_pton6(src, dst));  
        default:  
                return (-1);  
        }  
        /* NOTREACHED */  
}  
  
/* int 
 * inet_pton4(src, dst) 
 *      like inet_aton() but without all the hexadecimal and shorthand. 
 * return: 
 *      1 if `src' is a valid dotted quad, else 0. 
 * notice: 
 *      does not touch `dst' unless it's returning 1. 
 * author: 
 *      Paul Vixie, 1996. 
 */  
static int  
inet_pton4(const char *src, u_char *dst)  
{  
        static const char digits[] = "0123456789";  
        int saw_digit, octets, ch;  
#define NS_INADDRSZ     4  
        u_char tmp[NS_INADDRSZ], *tp;  
  
        saw_digit = 0;  
        octets = 0;  
        *(tp = tmp) = 0;  
        while ((ch = *src++) != '\0') {  
                const char *pch;  
  
                if ((pch = strchr(digits, ch)) != NULL) {  
                        u_int new = *tp * 10 + (pch - digits);  
  
                        if (saw_digit && *tp == 0)  
                                return (0);  
                        if (new > 255)  
                                return (0);  
                        *tp = new;  
                        if (!saw_digit) {  
                                if (++octets > 4)  
                                        return (0);  
                                saw_digit = 1;  
                        }  
                } else if (ch == '.' && saw_digit) {  
                        if (octets == 4)  
                                return (0);  
                        *++tp = 0;  
                        saw_digit = 0;  
                } else  
                        return (0);  
        }  
        if (octets < 4)  
                return (0);  
        memcpy(dst, tmp, NS_INADDRSZ);  
        return (1);  
}  
   
/* int 
 * inet_pton6(src, dst) 
 *      convert presentation level address to network order binary form. 
 * return: 
 *      1 if `src' is a valid [RFC1884 2.2] address, else 0. 
 * notice: 
 *      (1) does not touch `dst' unless it's returning 1. 
 *      (2) :: in a full address is silently ignored. 
 * credit: 
 *      inspired by Mark Andrews. 
 * author: 
 *      Paul Vixie, 1996. 
 */  
static int  
inet_pton6(const char *src, u_char *dst)  
{  
        static const char xdigits_l[] = "0123456789abcdef",  
                xdigits_u[] = "0123456789ABCDEF";  
#define NS_IN6ADDRSZ    16  
#define NS_INT16SZ      2  
        u_char tmp[NS_IN6ADDRSZ], *tp, *endp, *colonp;  
        const char *xdigits, *curtok;  
        int ch, seen_xdigits;  
        u_int val;  
   
        memset((tp = tmp), '\0', NS_IN6ADDRSZ);  
        endp = tp + NS_IN6ADDRSZ;  
        colonp = NULL;  
        /* Leading :: requires some special handling. */  
        if (*src == ':')  
                if (*++src != ':')  
                        return (0);  
        curtok = src;  
        seen_xdigits = 0;  
        val = 0;  
        while ((ch = *src++) != '\0') {  
                const char *pch;  
   
                if ((pch = strchr((xdigits = xdigits_l), ch)) == NULL)  
                        pch = strchr((xdigits = xdigits_u), ch);  
                if (pch != NULL) {  
                        val <<= 4;  
                        val |= (pch - xdigits);  
                        if (++seen_xdigits > 4)  
                                return (0);  
                        continue;  
                }  
                if (ch == ':') {  
                        curtok = src;  
                        if (!seen_xdigits) {  
                                if (colonp)  
                                        return (0);  
                                colonp = tp;  
                                continue;  
                        } else if (*src == '\0') {  
                                return (0);  
                        }  
                        if (tp + NS_INT16SZ > endp)  
                                return (0);  
                        *tp++ = (u_char) (val >> 8) & 0xff;  
                        *tp++ = (u_char) val & 0xff;  
                        seen_xdigits = 0;  
                        val = 0;  
                        continue;  
                }  
                if (ch == '.' && ((tp + NS_INADDRSZ) <= endp) &&  
                    inet_pton4(curtok, tp) > 0) {  
                        tp += NS_INADDRSZ;  
                        seen_xdigits = 0;  
                        break;  /*%< '\\' was seen by inet_pton4(). */  
                }  
                return (0);  
        }  
        if (seen_xdigits) {  
                if (tp + NS_INT16SZ > endp)  
                        return (0);  
                *tp++ = (u_char) (val >> 8) & 0xff;  
                *tp++ = (u_char) val & 0xff;  
        }  
        if (colonp != NULL) {  
                /* 
                 * Since some memmove()'s erroneously fail to handle 
                 * overlapping regions, we'll do the shift by hand. 
                 */  
                const int n = tp - colonp;  
                int i;  
   
                if (tp == endp)  
                        return (0);  
                for (i = 1; i <= n; i++) {  
                        endp[- i] = colonp[n - i];  
                        colonp[n - i] = 0;  
                }  
                tp = endp;  
        }  
        if (tp != endp)  
                return (0);  
        memcpy(dst, tmp, NS_IN6ADDRSZ);  
        return (1);  
}


#define BigLittleSwap32(A)  ((((uint32)(A) & 0xff000000) >> 24) |   \
                             (((uint32)(A) & 0x00ff0000) >> 8) |    \
                             (((uint32)(A) & 0x0000ff00) << 8) |    \
                             (((uint32)(A) & 0x000000ff) << 24))     

// big-endian 1; little-endian 0
int checkCPUendian(void)  
{  
        union{
                unsigned int  i;  
                unsigned char s[4];  
        }c;  
     
        c.i = 0x12345678;  

        return (0x12 == c.s[0]);  
}  


unsigned int ntohl(unsigned int n)  
{  
        return checkCPUendian() ? n : BigLittleSwap32(n);  
}  

#endif



