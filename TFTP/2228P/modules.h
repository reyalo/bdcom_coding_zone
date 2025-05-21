#ifndef _IFM_MODULES_H
#define _IFM_MODULES_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef unsigned short MODULE_TYPE;

#define MODULE_TYPE_LOWERBOUND		(0)
#define MODULE_TYPE_NONE			(0)

/* System Modules range:[1,30]*/
#define MODULE_TYPE_SYSTEM			(1)
#define MODULE_TYPE_HAL				(2)
#define MODULE_TYPE_TERMINAL		(3)
#define MODULE_TYPE_CMD				(4)
#define MODULE_TYPE_SYSLOG			(5)
#define MODULE_TYPE_JOB				(6)
#define MODULE_TYPE_USER			(7)
#define MODULE_TYPE_GPL				(8)
#define MODULE_TYPE_CTRLRELAY		(9)
#define MODULE_TYPE_RPC				(10)
#define MODULE_TYPE_OIR				(11)
#define MODULE_TYPE_RESOURCE		(12)
#define MODULE_TYPE_REDN			(13)
#define MODULE_TYPE_BSPS			(14)
#define MODULE_TYPE_PUB				(15)
#define MODULE_TYPE_DRIVERS			(16)
#define MODULE_TYPE_DM              (17)
#define MODULE_TYPE_OLT				(18)
#define MODULE_TYPE_OLT_REDN		(19)
#define MODULE_TYPE_FILESYS			(20)
#define MODULE_TYPE_HOTFIX			(21)

#define MODULE_TYPE_IMST			(22)
#define MODULE_TYPE_PHY_REDN		(23)
#define MODULE_TYPE_SDK				(24)

#define MODULE_TYPE_PSG				(25)

#define MODULE_TYPE_WQ				(26)
#define MODULE_TYPE_TIGER			(27)
#define MODULE_TYPE_HAL_EARLY		(28)
#define MODULE_TYPE_CORE_DUMP       (29)

/* Data Link Modules range:[31,60]*/
#define MODULE_TYPE_MAC				(31)
#define MODULE_TYPE_ETHERNET		MODULE_TYPE_MAC
#define MODULE_TYPE_DOT1X			(32)
#define MODULE_TYPE_STP				(33)
#define MODULE_TYPE_LACP			(34)
#define MODULE_TYPE_GARP			(35)
#define MODULE_TYPE_IGMP_SNOOPING   (36)
#define MODULE_TYPE_TUNNEL			(37)
#define MODULE_TYPE_GVRP			(38)
#define MODULE_TYPE_GMRP			(39)
#define MODULE_TYPE_VLANDB			(40)
#define MODULE_TYPE_UDLD			(41)
#define MODULE_TYPE_DHCPR			(42)
#define MODULE_TYPE_STACK           (43)
#define MODULE_TYPE_NATM            (44)
#define MODULE_TYPE_EAPS			(45)
#define MODULE_TYPE_OAM				(46)
#define MODULE_TYPE_LLC				(47)
#define MODULE_TYPE_VPDN			(48)
#define MODULE_TYPE_PPP             (49)
#define MODULE_TYPE_CFM				(50)
#define MODULE_TYPE_LLDP 			(51)
#define MODULE_TYPE_EBCMP			(52)
#define MODULE_TYPE_MLD_SNOOPING	(53)
#define MODULE_TYPE_ERPS			(54)
#define MODULE_TYPE_ETHOAM			(55)
#define MODULE_TYPE_PTP				(56)
#define MODULE_TYPE_MVC 			(57)
#define MODULE_TYPE_Y1731			(58)
#define MODULE_TYPE_MACFF			(59)
#define MODULE_TYPE_RHOS			(60)
#define MODULE_TYPE_PPPOE_IA		(61)
#define MODULE_TYPE_FLEXLINK		(62)
#define MODULE_TYPE_URPF			(63)   /*frank urpf*/
#define MODULE_TYPE_MEAPS           (64)
#define MODULE_TYPE_DH6SN           (65)
#define MODULE_TYPE_LOOPBACK_DETECT (66)
/* Misc modules 1, reserved for old versions[61,70]*/
#define MODULE_TYPE_FILE_SYNC       (67)
#define MODULE_TYPE_ELPS			(68)
#define MODULE_TYPE_STACK_MNGT		(69)
#define MODULE_TYPE_SGNP			(70)
/* TCP/IP Modules(Network Layer Modules) range:[71,90]*/
#define MODULE_TYPE_IPV6IP			(71)
#define MODULE_TYPE_NETFLOW			(72)
#define MODULE_TYPE_SFLOW			(73)
#define MODULE_TYPE_ACCESSLIST		(74)
#define MODULE_TYPE_IPV4FLOW		(79)
#define MODULE_TYPE_IPV4			(80)
#define MODULE_TYPE_ARP				(81)
#define MODULE_TYPE_NAT				(82)
#define MODULE_TYPE_IPACCESSLIST	(83)
#define MODULE_TYPE_NULLINTERFACE	(84)
#define MODULE_TYPE_LOOPBACK		(85)
#define MODULE_TYPE_IPSEC			(86)
#define MODULE_TYPE_OSI				(87)
#define MODULE_TYPE_IPV6			(88)
#define MODULE_TYPE_ND				(89)
#define MODULE_TYPE_IPV6ACL			(90)

/* Routing Modules range:[91, 130]*/
#define MODULE_TYPE_ROUTING			(91)
#define MODULE_TYPE_MRT				(92)
#define MODULE_TYPE_RIP				(93)
#define MODULE_TYPE_OSPF			(94)
#define MODULE_TYPE_EIGRP			(95)
#define MODULE_TYPE_BGP				(96)
#define MODULE_TYPE_ROUTEMAP		(97)
#define MODULE_TYPE_BGPLIST			(98)
#define MODULE_TYPE_POLICYMAP		(99)
#define MODULE_TYPE_PREFIXLIST		(100)
#define MODULE_TYPE_PBR				(101)
#define MODULE_TYPE_MSTATIC			(102)
#define MODULE_TYPE_OLNK			(103)
#define MODULE_TYPE_DVMRP			(104)
#define MODULE_TYPE_PIMDM			(105)
#define MODULE_TYPE_PIMSM			(106)
#define MODULE_TYPE_IGMP			(107)
#define MODULE_TYPE_ISIS			(108)
#define MODULE_TYPE_RTLIB			(109)
#define MODULE_TYPE_RTV6			(110)	/* Commented by XL.Zhou in 2006.01.17 14:35:23 */
#define MODULE_TYPE_RIPNG			(111)
#define MODULE_TYPE_OSPF6			(112)	/* Commented by XL.Zhou in 2006.03.07 15:41:36 */
#define MODULE_TYPE_RTV6LC			(113)	/*  */
#define MODULE_TYPE_BFD				(114)
#define MODULE_TYPE_MSDP			(115)
#define MODULE_TYPE_MRP				(116)
#define MODULE_TYPE_TMRP			(117)
#define MODULE_TYPE_TRILL			(118)
#define MODULE_TYPE_FIP_SNOOPING    (119)
#define MODULE_TYPE_OLT_GPON        (120)
#define MODULE_TYPE_OCS_OLT         (121)
#define MODULE_TYPE_OLT_GPON_REDN   (122)
#define MODULE_TYPE_HIPERRING		(123)


/* MPLS Modules range:[130, 150]*/
#define MODULE_TYPE_MPLS            (131)
#define MODULE_TYPE_FR              (132)
#define MODULE_TYPE_LM              (134)
#define MODULE_TYPE_LDP             (135)
#define MODULE_TYPE_HDLC            (136)
#define MODULE_TYPE_FM              (137)
#define MODULE_TYPE_MPLS_L2VPN		(138)
#define MODULE_TYPE_X25LAPB			(139)
#define MODULE_TYPE_MPLSTE          (140)
#define MODULE_TYPE_RSVPTE          (141)
#define MODULE_TYPE_TC              (142)

/* Apps Modules range:[151,200]*/
#define MODULE_TYPE_NBNS			(151)
#define MODULE_TYPE_HSRP			(152)
#define MODULE_TYPE_TACACSPLUS		(153)
#define MODULE_TYPE_RADIUS			(154)
#define MODULE_TYPE_CDP				(155)
#define MODULE_TYPE_TFTP			(156)
#define MODULE_TYPE_SNMP			(157)
#define MODULE_TYPE_HTTP			(158)
#define MODULE_TYPE_TELNET			(159)
#define MODULE_TYPE_RLOGIN			(160)
#define MODULE_TYPE_DHCP			(161)
#define MODULE_TYPE_SNTP			(162)
#define MODULE_TYPE_TIMERANGE		(163)
#define MODULE_TYPE_VRRP			(164)
#define MODULE_TYPE_RMON			(165)
#define MODULE_TYPE_DHCP_SERVER		(166)
#define MODULE_TYPE_QOS 			(167)
#define MODULE_TYPE_BCMP			(168)
#define MODULE_TYPE_WEBA            (169)
#define MODULE_TYPE_SSHD            (170)
#define MODULE_TYPE_SCRIPT          (171)
#define MODULE_TYPE_VRF				(172)
#define MODULE_TYPE_FTP             (173)
#define MODULE_TYPE_DHCP6			(174)

#define MODULE_TYPE_CHASSIS			(175)
/* Misc Modules */
#define MODULE_TYPE_MNGTPT			(176)
#define MODULE_TYPE_VPHY  			(177)
#define MODULE_TYPE_GRE				(178)
#define MODULE_TYPE_PPPOE 			(179)
#define MODULE_TYPE_FTPD			(180)
#define MODULE_TYPE_RNP				(181)
#define MODULE_TYPE_TMP				(182)
#define MODULE_TYPE_NTP				(183)
#define MODULE_TYPE_ESMC			(184)
#define MODULE_TYPE_LWNAT			(189)
#define MODULE_TYPE_MAT				(190)
#define MODULE_TYPE_ONU				(191)/*提交代码时调整为191*/
#define MODULE_TYPE_ONUVLAN			(192)
#define MODULE_TYPE_ONUHAL			(193)
#define MODULE_TYPE_ONUQOS			(194)
#define MODULE_TYPE_ONUMCST			(195)
#define MODULE_TYPE_ONUPRECFG		(196)
#define MODULE_TYPE_SQLITE			(197)
#define MODULE_TYPE_CTCSTACK		(198)
#define MODULE_TYPE_HTTPWEB_RES     (199)

/* zhouxie add for 12ge_cvmx */
#define MODULE_TYPE_GBSC			(200)
#define MODULE_TYPE_LAYER7FILTER	(201)

#define MODULE_TYPE_UART			(202)

#define MODULE_TYPE_EDDP			(203)
#define MODULE_TYPE_SHELFINTER		(204)
#define MODULE_TYPE_CARDSYS			(205)
#define MODULE_TYPE_RTA				(206)
#define MODULE_TYPE_MAC_AUTH        (207)
#define MODULE_TYPE_SOCKET_PROXY	(208)
#define MODULE_TYPE_IEC61850SERVER  (209)
#define MODULE_TYPE_LIBIEC61850     (210)
#define MODULE_TYPE_HAL_BROADCOM	(211)
#define MODULE_TYPE_HAL_CORTINA		(212)
#define MODULE_TYPE_HAL_CTC			(213)
#define MODULE_TYPE_HAL_MARVELL		(214)
#define MODULE_TYPE_HAL_PMC			(215)
#define MODULE_TYPE_HAL_REALTEK		(216)
#define MODULE_TYPE_HAL_ROBO		(217)
#define MODULE_TYPE_HAL_TIGER		(218)
#define MODULE_TYPE_HAL_VTSS		(219)
#define MODULE_TYPE_SUGON_MANAGE	(220)
#define MODULE_TYPE_HAL_PHY			(221)
#define MODULE_TYPE_IPWC			(222)
#define MODULE_TYPE_UNRARLIB		(223)
#define MODULE_TYPE_POE				(224)
#define MODULE_TYPE_HAL_MAPLE       (225)
#define MODULE_TYPE_PORTAL_AUTH     (226)
#define MODULE_TYPE_HAL_TIGER4		(227)
#define MOUDLE_TYPE_HAL_PHYAPPL		(228)
/*for linux, user module interface*/
#define MODULE_TYPE_BDUSR			(229)
#define MODULE_TYPE_REDBOX          (230)
#define MODULE_TYPE_DIM 			(231)
#define MODULE_TYPE_REDBOX_FPGA		(232)
#define MODULE_TYPE_DCB            (233)
#define MODULE_TYPE_GPS				(237)
#define MODULE_TYPE_TIME            (238)
#define MODULE_TYPE_DRIVERS_USER     (239)
#define MODULE_TYPE_EPXRING         (240)

#define MODULE_TYPE_VFC				(241)
#define MODULE_TYPE_FCOE			(242)
#define MODULE_TYPE_BDIP			(243)
#define MODULE_TYPE_SOLTECH         (244)
#define MODULE_TYPE_HAL_SFP			(245)
#define MODULE_TYPE_SMTDP			(246)
#define MODULE_TYPE_HIK_ODM			(247)
#define MODULE_TYPE_CRYPTO			(248)
#define MODULE_TYPE_MQTTC			(249)
#define MODULE_TYPE_TFTPD			(250)

#define MODULE_TYPE_UPPERBOUND		(255)


/*system startup phase definition*/
#define SYSTEM_STARTUP_CONFIG			0
#define SYSTEM_PREPARE_CR				1
#define SYSTEM_ENABLE_FILESYNC			2
#define SYSTEM_PREPARE_RPC				3
#define SYSTEM_RUNNING					4
#define SYSTEM_BACKUP_START				5
#define SYSTEM_SYNC_SMOOTH				6
#define SYSTEM_SYNC_STARTUP				7

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _LIBDEV_MODULES_H */

