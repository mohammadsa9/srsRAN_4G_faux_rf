/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2021 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSRAN_RA_UL_NR_H
#define SRSRAN_RA_UL_NR_H

#include "dci_nr.h"
#include "srsran/config.h"
#include "srsran/phy/phch/phch_cfg_nr.h"
#include "srsran/phy/phch/pucch_cfg_nr.h"
#include "uci_cfg_nr.h"

/**
 * @brief Calculates the PUSCH time resource allocation and stores it in the provided PUSCH NR grant.
 *
 * @remark Defined by TS 38.214 V15.10.0 section 6.1.2.1.1 Determination of the resource allocation table to be used for
 * PUSCH
 *
 * @param cfg Flattened PUSCH configuration provided from higher layers
 * @param rnti_type Type of the RNTI of the corresponding DCI
 * @param ss_type Type of the SS for PDCCH
 * @param coreset_id CORESET identifier associated with the PDCCH transmission
 * @param m Time domain resource assignment field value m provided in DCI
 * @param[out] Provides grant pointer to fill
 * @return Returns SRSRAN_SUCCESS if the provided allocation is valid, otherwise it returns SRSRAN_ERROR code
 */
SRSRAN_API int srsran_ra_ul_nr_time(const srsran_sch_hl_cfg_nr_t*    cfg,
                                    const srsran_rnti_type_t         rnti_type,
                                    const srsran_search_space_type_t ss_type,
                                    const uint32_t                   coreset_id,
                                    const uint8_t                    m,
                                    srsran_sch_grant_nr_t*           grant);

/**
 * @brief Calculates the PUSCH time resource default A and stores it in the provided PUSCH NR grant.
 *
 * @remark Defined by TS 38.214 V15.10.0 Table 6.1.2.1.1-2: Default PUSCH time domain resource allocation A for normal
 * CP
 *
 * @param scs_cfg Sub-carrier spacing configuration for PUSCH (μ)
 * @param m Time domain resource assignment field value m of the DCI
 * @param[out] grant PUSCH grant
 * @return Returns SRSRAN_SUCCESS if the provided allocation is valid, otherwise it returns SRSRAN_ERROR code
 */
SRSRAN_API int
srsran_ra_ul_nr_pusch_time_resource_default_A(uint32_t scs_cfg, uint32_t m, srsran_sch_grant_nr_t* grant);

/**
 * @brief Calculates the number of PUSCH-DMRS CDM groups without data for DCI format 0_0
 *
 * @remark Defined by TS 38.214 V15.10.0 6.2.2 UE DM-RS transmission procedure
 *
 * @param cfg PUSCH NR configuration by upper layers
 * @param[out] grant Provides grant pointer to fill
 * @return Returns SRSRAN_SUCCESS if the provided data is valid, otherwise it returns SRSRAN_ERROR code
 */
SRSRAN_API int srsran_ra_ul_nr_nof_dmrs_cdm_groups_without_data_format_0_0(const srsran_sch_cfg_nr_t* cfg,
                                                                           srsran_sch_grant_nr_t*     grant);

/**
 * @brief Calculates the ratio of PUSCH EPRE to DM-RS EPRE
 *
 * @remark Defined by TS 38.214 V15.10.0 Table 6.2.2-1: The ratio of PUSCH EPRE to DM-RS EPRE
 *
 * @param[out] grant Provides grant pointer to fill
 * @return Returns SRSRAN_SUCCESS if the provided data is valid, otherwise it returns SRSRAN_ERROR code
 */
SRSRAN_API int srsran_ra_ul_nr_dmrs_power_offset(srsran_sch_grant_nr_t* grant);

/**
 * @brief Calculates the minimum number of PRB required for transmitting NR-PUCCH Format 2, 3 or 4
 * @remark Based in TS 38.213 9.2.5.1 UE procedure for multiplexing HARQ-ACK or CSI and SR in a PUCCH
 * @return The number of PRB if the provided configuration is valid, SRSRAN_ERROR code otherwise
 */
SRSRAN_API int srsran_ra_ul_nr_pucch_format_2_3_min_prb(const srsran_pucch_nr_resource_t* resource,
                                                        const srsran_uci_cfg_nr_t*        uci_cfg);

/**
 * @brief Calculates the PUSCH frequency resource allocation and stores it in the provided PUSCH NR grant.
 *
 * @remark Defined by TS 38.214 V15.10.0 section 5.1.2.2
 * @param carrier Carrier information
 * @param cfg PDSCH NR configuration by upper layers
 * @param dci_dl Unpacked DCI used to schedule the PDSCH grant
 * @param[out] grant Provides grant pointer to fill
 * @return SRSRAN_SUCCESS if the provided data is valid, SRSRAN_ERROR code otherwise
 */
SRSRAN_API int srsran_ra_ul_nr_freq(const srsran_carrier_nr_t*    carrier,
                                    const srsran_sch_hl_cfg_nr_t* cfg,
                                    const srsran_dci_ul_nr_t*     dci_ul,
                                    srsran_sch_grant_nr_t*        grant);

/**
 * @brief Selects a valid PUCCH resource for transmission
 * @param pucch_cfg PUCCH configuration from upper layers
 * @param uci_cfg Uplink Control information configuration (and PDCCH context)
 * @param[out] resource Selected resource for transmitting PUCCH
 * @return SRSRAN_SUCCESS if provided configuration is valid, SRSRAN_ERROR code otherwise
 */
SRSRAN_API int srsran_ra_ul_nr_pucch_resource(const srsran_pucch_nr_hl_cfg_t* pucch_cfg,
                                              const srsran_uci_cfg_nr_t*      uci_cfg,
                                              srsran_pucch_nr_resource_t*     resource);

/**
 * @brief Computes the number of SR bits
 * @param K Number of SR transmission opportunities, including negative
 * @return The number of bits according to the number of SRs
 */
SRSRAN_API uint32_t srsran_ra_ul_nr_nof_sr_bits(uint32_t K);

#endif // SRSRAN_RA_UL_NR_H