/***********************************************************
* title_name		: Player Settings Module
* date_created		: 2020.01.19
* filename			: PythonPlayerSettingsModule.h
* author			: VegaS
* version_actual	: Version 3.8
*/
#pragma once
#include "InstanceBase.h"
#include "PythonItem.h"
#include <cstdint>
#include <initializer_list>
#include <array>
#include <PythonPlayerSettingsModuleLib.h>

static const auto EMOTICON_PATH = "d:/ymir work/effect/etc/emoticon/";
static const auto NPC_LIST_FILE_NAME = "npclist.txt";

enum
{
	HORSE_SKILL_WILDATTACK = CRaceMotionData::NAME_SKILL + 121,
	HORSE_SKILL_CHARGE = CRaceMotionData::NAME_SKILL + 122,
	HORSE_SKILL_SPLASH = CRaceMotionData::NAME_SKILL + 123,

	GUILD_SKILL_DRAGONBLOOD = CRaceMotionData::NAME_SKILL + 101,
	GUILD_SKILL_DRAGONBLESS = CRaceMotionData::NAME_SKILL + 102,
	GUILD_SKILL_BLESSARMOR = CRaceMotionData::NAME_SKILL + 103,
	GUILD_SKILL_SPPEDUP = CRaceMotionData::NAME_SKILL + 104,
	GUILD_SKILL_DRAGONWRATH = CRaceMotionData::NAME_SKILL + 105,
	GUILD_SKILL_MAGICUP = CRaceMotionData::NAME_SKILL + 106,

	COMBO_TYPE_1 = 0,
	COMBO_TYPE_2 = 1,
	COMBO_TYPE_3 = 2,

	COMBO_INDEX_1 = 0,
	COMBO_INDEX_2 = 1,
	COMBO_INDEX_3 = 2,
	COMBO_INDEX_4 = 3,
	COMBO_INDEX_5 = 4,
	COMBO_INDEX_6 = 5,
};

//! @brief skill index of passive guild skill
static const std::vector<uint8_t> m_vecPassiveGuildSkillIndex = { 151, };
//! @brief skill index of active guild skill
static const std::vector<uint8_t> m_vecActiveGuildSkillIndex = { 152, 153, 154, 155, 156, 157, };

//! @brief Skill index vector with for all characters
static const std::vector<TSkillIndex> m_vecSkillIndex =
{
	{
		NRaceData::JOB_WARRIOR,
		{
			{ 1, { 1, 2, 3, 4, 5, 6, 0, 0, 137, 0, 138, 0, 139, 0, } },
			{ 2, { 16, 17, 18, 19, 20, 21, 0, 0, 137, 0, 138, 0, 139, 0, } },
		},
		{ 122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 0, 0 }
	},
	{
		NRaceData::JOB_ASSASSIN,
		{
			{ 1, { 31, 32, 33, 34, 35, 36, 0, 0, 137, 0, 138, 0, 139, 0, 140 } },
			{ 2, { 46, 47, 48, 49, 50, 51, 0, 0, 137, 0, 138, 0, 139, 0, 140 } },
		},
		{ 122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 0, 0 }
	},
	{
		NRaceData::JOB_SURA,
		{
			{ 1, { 61, 62, 63, 64, 65, 66, 0, 0, 137, 0, 138, 0, 139, 0 } },
			{ 2, { 76, 77, 78, 79, 80, 81, 0, 0, 137, 0, 138, 0, 139, 0 } },
		},
		{ 122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 0, 0 }
	},
	{
		NRaceData::JOB_SHAMAN,
		{
			{ 1, { 91, 92, 93, 94, 95, 96, 0, 0, 137, 0, 138, 0, 139, 0 } },
			{ 2, { 106, 107, 108, 109, 110, 111, 0, 0, 137, 0, 138, 0, 139, 0 } },
		},
		{ 122, 123, 121, 124, 125, 129, 0, 0, 130, 131, 0, 0 }
	},
};

/**
* \brief Concatenate unlimited string arguments into one string.
*/
static auto string_path(const std::initializer_list<std::string> & args) -> std::string
{
	std::string stPathName;
	for (const auto & it : args)
		stPathName.append(it);
	return stPathName;
}

/**
* \brief
*/
static const std::vector<THorse> m_vecHorse =
{
	{ NRaceData::JOB_WARRIOR, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::MODE_HORSE_TWOHAND_SWORD } },
	{ NRaceData::JOB_ASSASSIN, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD, CRaceMotionData::MODE_HORSE_DUALHAND_SWORD } },
	{ NRaceData::JOB_SURA, { CRaceMotionData::MODE_HORSE_ONEHAND_SWORD } },
	{ NRaceData::JOB_SHAMAN, { CRaceMotionData::MODE_HORSE_FAN, CRaceMotionData::MODE_HORSE_BELL } },
};

/**
* \brief
*/
static const std::vector<TSkillData> m_vecSkillFileName =
{
	{ NRaceData::JOB_WARRIOR, { "samyeon", "palbang", "jeongwi", "geomgyeong", "tanhwan", "gihyeol", "gigongcham", "gyeoksan", "daejin", "cheongeun", "geompung", "noegeom" }, },
	{ NRaceData::JOB_ASSASSIN, { "amseup", "gungsin", "charyun", "eunhyeong", "sangong", "seomjeon", "yeonsa", "gwangyeok", "hwajo", "gyeonggong", "dokgigung", "seomgwang" }, },
	{ NRaceData::JOB_SHAMAN,
		{ "bipabu", "yongpa", "paeryong", "hosin", "boho", "gicheon", "noejeon", "byeorak", "pokroe", "jeongeop", "kwaesok", "jeungryeok" },
		{ "bipabu", "yongpa", "paeryong", "hosin_target", "boho_target", "gicheon_target", "noejeon", "byeorak", "pokroe", "jeongeop_target", "kwaesok_target", "jeungryeok_target" }
	},
	{ NRaceData::JOB_SURA, { "swaeryeong", "yonggwon", "gwigeom", "gongpo", "jumagap", "pabeop", "maryeong", "hwayeom", "muyeong", "heuksin", "tusok", "mahwan" }, },
};

/**
* \brief Cached effects
*/
static const std::vector<TCacheEffect> m_vecEffects =
{
	{ CInstanceBase::EFFECT_DUST, "", "d:/ymir work/effect/etc/dust/dust.mse", true },
	{ CInstanceBase::EFFECT_HORSE_DUST, "", "d:/ymir work/effect/etc/dust/running_dust.mse", true },
	{ CInstanceBase::EFFECT_HIT, "", "d:/ymir work/effect/hit/blow_1/blow_1_low.mse", true },
	{ CInstanceBase::EFFECT_HPUP_RED, "", "d:/ymir work/effect/etc/recuperation/drugup_red.mse", true },
	{ CInstanceBase::EFFECT_SPUP_BLUE, "", "d:/ymir work/effect/etc/recuperation/drugup_blue.mse", true },
	{ CInstanceBase::EFFECT_SPEEDUP_GREEN, "", "d:/ymir work/effect/etc/recuperation/drugup_green.mse", true },
	{ CInstanceBase::EFFECT_DXUP_PURPLE, "", "d:/ymir work/effect/etc/recuperation/drugup_purple.mse", true },
	{ CInstanceBase::EFFECT_AUTO_HPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_red.mse", true },
	{ CInstanceBase::EFFECT_AUTO_SPUP, "", "d:/ymir work/effect/etc/recuperation/autodrugup_blue.mse", true },
	{ CInstanceBase::EFFECT_RAMADAN_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item1.mse", true },
	{ CInstanceBase::EFFECT_HALLOWEEN_CANDY_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item2.mse", true },
	{ CInstanceBase::EFFECT_HAPPINESS_RING_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item3.mse", true },
	{ CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item4.mse", true },
	{ CInstanceBase::EFFECT_PENETRATE, "Bip01", "d:/ymir work/effect/hit/gwantong.mse", true },
	{ CInstanceBase::EFFECT_FIRECRACKER, "", "d:/ymir work/effect/etc/firecracker/newyear_firecracker.mse", true },
	{ CInstanceBase::EFFECT_SPIN_TOP, "", "d:/ymir work/effect/etc/firecracker/paing_i.mse", true },
	{ CInstanceBase::EFFECT_SELECT, "", "d:/ymir work/effect/etc/click/click_select.mse", true },
	{ CInstanceBase::EFFECT_TARGET, "", "d:/ymir work/effect/etc/click/click_glow_select.mse", true },
	{ CInstanceBase::EFFECT_STUN, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun.mse", true },
	{ CInstanceBase::EFFECT_CRITICAL, "Bip01 R Hand", "d:/ymir work/effect/hit/critical.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_TARGET, "", "d:/ymir work/effect/affect/damagevalue/target.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_NOT_TARGET, "", "d:/ymir work/effect/affect/damagevalue/nontarget.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE, "", "d:/ymir work/effect/affect/damagevalue/damage.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_SELFDAMAGE2, "", "d:/ymir work/effect/affect/damagevalue/damage_1.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_POISON, "", "d:/ymir work/effect/affect/damagevalue/poison.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_MISS, "", "d:/ymir work/effect/affect/damagevalue/miss.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_TARGETMISS, "", "d:/ymir work/effect/affect/damagevalue/target_miss.mse", true },
	{ CInstanceBase::EFFECT_PVP_WIN, "", ":/ymir work/effect/etc/pvp/pvp_victory.mse", true },
	{ CInstanceBase::EFFECT_PVP_OPEN1, "", "d:/ymir work/effect/etc/pvp/pvp_open1.mse", true },
	{ CInstanceBase::EFFECT_PVP_OPEN2, "", "d:/ymir work/effect/etc/pvp/pvp_open2.mse", true },
	{ CInstanceBase::EFFECT_PVP_BEGIN1, "", "d:/ymir work/effect/etc/pvp/pvp_begin1.mse", true },
	{ CInstanceBase::EFFECT_PVP_BEGIN2, "", "d:/ymir work/effect/etc/pvp/pvp_begin2.mse", true },
	{ CInstanceBase::EFFECT_DAMAGE_CRITICAL, "", "d:/ymir work/effect/affect/damagevalue/critical.mse", true },
	{ CInstanceBase::EFFECT_AGGREGATE_MONSTER, "", "d:/ymir work/effect/aggregate/aggregate_monster.mse", true },
	{ CInstanceBase::EFFECT_LEVELUP_ON_14_FOR_GERMANY, "", "season1/effect/paymessage_warning.mse", true },
	{ CInstanceBase::EFFECT_LEVELUP_UNDER_15_FOR_GERMANY, "", "season1/effect/paymessage_decide.mse", true },
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE1, "", "d:/ymir work/effect/hit/percent_damage1.mse", true },
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE2, "", "d:/ymir work/effect/hit/percent_damage2.mse", true },
	{ CInstanceBase::EFFECT_PERCENT_DAMAGE3, "", "d:/ymir work/effect/hit/percent_damage3.mse", true },
	{ CInstanceBase::EFFECT_DEFENSE_WAVE_LASER, "", "d:/ymir work/effect/monster2/defense_mini_laser.mse", true},

#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	{ CInstanceBase::EFFECT_ACCE_SUCCEDED, "", "d:/ymir work/effect/etc/buff/buff_item6.mse", true },
	{ CInstanceBase::EFFECT_ACCE_EQUIP, "", "d:/ymir work/effect/etc/buff/buff_item7.mse", true },
#endif

#ifdef ENABLE_TALISMAN_EFFECT
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_FIRE, "", "d:/ymir work/talisman/effect/talismano_fuoco.mse", true },
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_ICE, "", "d:/ymir work/talisman/effect/talismano_ghiaccio.mse", true },
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_WIND, "", "d:/ymir work/talisman/effect/talismano_vento.mse", true },
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_EARTH, "", "d:/ymir work/talisman/effect/talismano_terra.mse", true },
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_DARK, "", "d:/ymir work/talisman/effect/talismano_oscuro.mse", true },
		{ CInstanceBase::EFFECT_TALISMAN_EQUIP_ELEC, "", "d:/ymir work/talisman/effect/talismano_lampo.mse", true },
#endif

#ifdef ENABLE_OVER_KILL
	{ CInstanceBase::EFFECT_OVER_KILL_START, "", "d:/ymir work/effect/over_kill/double_kill.mse", true },
	{ CInstanceBase::EFFECT_OVER_KILL_START + 1	, "", "d:/ymir work/effect/over_kill/triple_kill.mse", true },
	{ CInstanceBase::EFFECT_OVER_KILL_START + 2	, "", "d:/ymir work/effect/over_kill/quadruple_kill.mse", true },
	{ CInstanceBase::EFFECT_OVER_KILL_START + 3	, "", "d:/ymir work/effect/over_kill/penta_kill.mse", true },
	{ CInstanceBase::EFFECT_OVER_KILL_END, "", "d:/ymir work/effect/over_kill/legendary_kill.mse", true },
#endif
#ifdef ENABLE_NEW_SELECT_INTERFACE
	{ CInstanceBase::EFFECT_SELECT_PG, "", "d:/ymir work/effect/select/select.mse", true },
#endif

#ifdef GUILD_RANK_EFFECT
	{ CInstanceBase::EFFECT_GUILD_RANK_1, "Bip01", "d:/ymir work/effect/etc/guild/ranker_1.mse", true },
	{ CInstanceBase::EFFECT_GUILD_RANK_2, "Bip01", "d:/ymir work/effect/etc/guild/ranker_2.mse", true },
	{ CInstanceBase::EFFECT_GUILD_RANK_3, "Bip01", "d:/ymir work/effect/etc/guild/ranker_3.mse", true },
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// No cache
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	{ CInstanceBase::EFFECT_SPAWN_APPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_appear.mse", false },
	{ CInstanceBase::EFFECT_SPAWN_DISAPPEAR, "Bip01", "d:/ymir work/effect/etc/appear_die/monster_die.mse", false },
	{ CInstanceBase::EFFECT_FLAME_ATTACK, "equip_right_hand", "d:/ymir work/effect/hit/blow_flame/flame_3_weapon.mse", false },
	{ CInstanceBase::EFFECT_FLAME_HIT, "", "d:/ymir work/effect/hit/blow_flame/flame_3_blow.mse", false },
	{ CInstanceBase::EFFECT_FLAME_ATTACH, "", "d:/ymir work/effect/hit/blow_flame/flame_3_body.mse", false },
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACK, "equip_right", "d:/ymir work/effect/hit/blow_electric/light_1_weapon.mse", false },
	{ CInstanceBase::EFFECT_ELECTRIC_HIT, "", "d:/ymir work/effect/hit/blow_electric/light_1_blow.mse", false },
	{ CInstanceBase::EFFECT_ELECTRIC_ATTACH, "", "d:/ymir work/effect/hit/blow_electric/light_1_body.mse", false },
	{ CInstanceBase::EFFECT_LEVELUP, "", "d:/ymir work/effect/etc/levelup_1/level_up.mse", false },
	{ CInstanceBase::EFFECT_SKILLUP, "", "d:/ymir work/effect/etc/skillup/skillup_1.mse", false },
	{ CInstanceBase::EFFECT_BOSS, "", "d:/ymir work/effect/loknarash_effect/boss_effect/boss_2.mse", false },
#ifdef ENABLE_SPECIAL_STATS_SYSTEM
	{ CInstanceBase::EFFECT_SPECIAL_STATUP, "", "d:/ymir work/effect/etc/skillup/skillup_1.mse", false },
#endif

	{ CInstanceBase::EFFECT_RUNA_WHITE_EQUIP, "", "d:/ymir work/effect/rune/buff_white.mse", false },
	{ CInstanceBase::EFFECT_RUNA_RED_EQUIP, "", "d:/ymir work/effect/rune/buff_red.mse", false },
	{ CInstanceBase::EFFECT_RUNA_BLUE_EQUIP, "", "d:/ymir work/effect/rune/buff_blue.mse", false },
	{ CInstanceBase::EFFECT_RUNA_YELLOW_EQUIP, "", "d:/ymir work/effect/rune/buff_yellow.mse", false },
	{ CInstanceBase::EFFECT_RUNA_BLACK_EQUIP, "", "d:/ymir work/effect/rune/buff_black.mse", false },
	{ CInstanceBase::EFFECT_RUNA_GREEN_EQUIP, "", "d:/ymir work/effect/rune/buff_green.mse", false },


	{ CInstanceBase::EFFECT_EMPIRE + 1, "Bip01", "d:/ymir work/effect/etc/empire/empire_A.mse", false },
	{ CInstanceBase::EFFECT_EMPIRE + 2, "Bip01", "d:/ymir work/effect/etc/empire/empire_B.mse", false },
	{ CInstanceBase::EFFECT_EMPIRE + 3, "Bip01", "d:/ymir work/effect/etc/empire/empire_C.mse", false },

	{ CInstanceBase::EFFECT_WEAPON + 1, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_sword_loop.mse", false },
	{ CInstanceBase::EFFECT_WEAPON + 2, "equip_right_hand", "d:/ymir work/pc/warrior/effect/geom_spear_loop.mse", false },

	{ CInstanceBase::EFFECT_AFFECT + 0, "Bip01", string_path({ "locale/en", "/effect/gm.mse" }), false },
	{ CInstanceBase::EFFECT_AFFECT + 3, "Bip01", "d:/ymir work/effect/hit/blow_poison/poison_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 4, "", "d:/ymir work/effect/affect/slow.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 5, "Bip01 Head", "d:/ymir work/effect/etc/stun/stun_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 6, "", "d:/ymir work/effect/etc/ready/ready.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 16, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 17, "", "d:/ymir work/pc/assassin/effect/gyeonggong_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 19, "Bip01 R Finger2", "d:/ymir work/pc/sura/effect/gwigeom_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 20, "", "d:/ymir work/pc/sura/effect/fear_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 21, "", "d:/ymir work/pc/sura/effect/jumagap_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 22, "", "d:/ymir work/pc/shaman/effect/3hosin_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 23, "", "d:/ymir work/pc/shaman/effect/boho_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 24, "", "d:/ymir work/pc/shaman/effect/10kwaesok_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 25, "", "d:/ymir work/pc/sura/effect/heuksin_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 26, "", "d:/ymir work/pc/sura/effect/muyeong_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 28, "Bip01", "d:/ymir work/effect/hit/blow_flame/flame_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 29, "Bip01 R Hand", "d:/ymir work/pc/shaman/effect/6gicheon_hand.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 30, "Bip01 L Hand", "d:/ymir work/pc/shaman/effect/jeungryeok_hand.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 32, "Bip01 Head", "d:/ymir work/pc/sura/effect/pabeop_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 33, "", "d:/ymir work/pc/warrior/effect/gyeokgongjang_loop.mse", false },
	//## 34 Polymoph
	{ CInstanceBase::EFFECT_AFFECT + 35, "", "d:/ymir work/effect/etc/guild_war_flag/flag_red.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 36, "", "d:/ymir work/effect/etc/guild_war_flag/flag_blue.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 37, "", "d:/ymir work/effect/etc/guild_war_flag/flag_yellow.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 38, "", "d:/ymir work/effect/affect/sleep_loop.mse", false },
#ifdef ENABLE_WOLFMAN_CHARACTER
	{ CInstanceBase::EFFECT_AFFECT + 42, "Bip01", "d:/ymir work/effect/hit/blow_poison/bleeding_loop.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 43, "Bip01", "d:/ymir work/effect/hit/blow_flame/flame_loop_w.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + 44, "", "d:/ymir work/pc3/common/effect/gyeokgongjang_loop_w.mse", false },
#endif
	{ CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_RESIST_BUFF, "", "d:/ymir work/my_work/effect/resistbuff.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_CRITICAL_BUFF, "Bip01 R Hand", "d:/ymir work/my_work/effect/criticalbuff.mse", false },
	{ CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_REFLECT_BUFF, "", "d:/ymir work/my_work/effect/reflectbuff.mse", false },

#ifdef ENABLE_DS_ACTIVE_LOOP
	{ CInstanceBase::EFFECT_AFFECT + CInstanceBase::AFFECT_DS_ACTIVE, "", "d:/ymir work/effect/etc/ds/kwaegeom_loop.mse", false },
#endif

#ifdef ENABLE_MELEY_LAIR_DUNGEON
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_STATUE1, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", false },
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_STATUE2, "", "d:/ymir work/effect/monster2/redd_moojuk.mse", false },
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_STATUE3, "", "d:/ymir work/effect/monster2/redd_moojuk_blue.mse", false },
	{ CInstanceBase::EFFECT_AFFECT+CInstanceBase::AFFECT_STATUE4, "", "d:/ymir work/effect/monster2/redd_moojuk_green.mse", false },
#endif

	{ CInstanceBase::EFFECT_REFINED + 1, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 2, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 3, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 4, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_b.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 5, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_b.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 6, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_b.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 7, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_f.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 8, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_f.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 9, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_f.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 10, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 11, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 12, "PART_WEAPON", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 13, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_7_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 14, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_8_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 15, "PART_WEAPON_LEFT", "D:/ymir work/pc/common/effect/sword/sword_9_s.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 16, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_7.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 17, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_8.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 18, "Bip01", "D:/ymir work/pc/common/effect/armor/armor_9.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 19, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-1.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 20, "Bip01", "D:/ymir work/pc/common/effect/armor/armor-4-2-2.mse", false },
#ifdef ENABLE_LVL115_ARMOR_EFFECT
	{ CInstanceBase::EFFECT_REFINED + 21, "Bip01", "D:/ymir work/pc/common/effect/armor/armorred.mse", false },
#endif
#ifdef ENABLE_ACCE_COSTUME_SYSTEM
	{ CInstanceBase::EFFECT_REFINED + 22, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
#endif
#if defined(ENABLE_AURA_SYSTEM) && defined(RENDER_SHINING)
	{ CInstanceBase::EFFECT_REFINED + 23, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 24, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 25, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 26, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 27, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
	{ CInstanceBase::EFFECT_REFINED + 28, "Bip01", "d:/ymir work/pc/common/effect/armor/acc_01.mse", false },
#endif
};

static const std::vector<TEmotion> m_vecEmotions = {
	{ CRaceMotionData::NAME_CLAP, "clap.msa" },
	{ CRaceMotionData::NAME_CHEERS_1, "cheers_1.msa" },
	{ CRaceMotionData::NAME_CHEERS_2, "cheers_2.msa" },
	{ CRaceMotionData::NAME_DANCE_1, "dance_1.msa" },
	{ CRaceMotionData::NAME_DANCE_2, "dance_2.msa" },
	{ CRaceMotionData::NAME_DANCE_3, "dance_3.msa" },
	{ CRaceMotionData::NAME_DANCE_4, "dance_4.msa" },
	{ CRaceMotionData::NAME_DANCE_5, "dance_5.msa" },
	{ CRaceMotionData::NAME_DANCE_6, "dance_6.msa" },
	{ CRaceMotionData::NAME_CONGRATULATION, "congratulation.msa" },
	{ CRaceMotionData::NAME_FORGIVE, "forgive.msa" },
	{ CRaceMotionData::NAME_ANGRY, "angry.msa" },
	{ CRaceMotionData::NAME_ATTRACTIVE, "attractive.msa" },
	{ CRaceMotionData::NAME_SAD, "sad.msa" },
	{ CRaceMotionData::NAME_SHY, "shy.msa" },
	{ CRaceMotionData::NAME_CHEERUP, "cheerup.msa" },
	{ CRaceMotionData::NAME_BANTER, "banter.msa" },
	{ CRaceMotionData::NAME_JOY, "joy.msa" },
	{ CRaceMotionData::NAME_FRENCH_KISS_WITH_WARRIOR, "french_kiss_with_warrior.msa" },
	{ CRaceMotionData::NAME_FRENCH_KISS_WITH_ASSASSIN, "french_kiss_with_assassin.msa" },
	{ CRaceMotionData::NAME_FRENCH_KISS_WITH_SURA, "french_kiss_with_sura.msa" },
	{ CRaceMotionData::NAME_FRENCH_KISS_WITH_SHAMAN, "french_kiss_with_shaman.msa" },
	{ CRaceMotionData::NAME_KISS_WITH_WARRIOR, "kiss_with_warrior.msa" },
	{ CRaceMotionData::NAME_KISS_WITH_ASSASSIN, "kiss_with_assassin.msa" },
	{ CRaceMotionData::NAME_KISS_WITH_SURA, "kiss_with_sura.msa" },
	{ CRaceMotionData::NAME_KISS_WITH_SHAMAN, "kiss_with_shaman.msa" },
	{ CRaceMotionData::NAME_SLAP_HIT_WITH_WARRIOR, "slap_hit.msa" },
	{ CRaceMotionData::NAME_SLAP_HIT_WITH_ASSASSIN, "slap_hit.msa" },
	{ CRaceMotionData::NAME_SLAP_HIT_WITH_SURA, "slap_hit.msa" },
	{ CRaceMotionData::NAME_SLAP_HIT_WITH_SHAMAN, "slap_hit.msa" },
	{ CRaceMotionData::NAME_SLAP_HURT_WITH_WARRIOR, "slap_hurt.msa" },
	{ CRaceMotionData::NAME_SLAP_HURT_WITH_ASSASSIN, "slap_hurt.msa" },
	{ CRaceMotionData::NAME_SLAP_HURT_WITH_SURA, "slap_hurt.msa" },
	{ CRaceMotionData::NAME_SLAP_HURT_WITH_SHAMAN, "slap_hurt.msa" },
#ifdef ENABLE_EXPRESSING_EMOTION
	{ CRaceMotionData::NAME_PUSHUP, "pushup.msa" },
	{ CRaceMotionData::NAME_PUSHUP, "pushup.msa" },
	{ CRaceMotionData::NAME_DANCE_7, "dance_7.msa" },
	{ CRaceMotionData::NAME_EXERCISE, "exercise.msa" },
	{ CRaceMotionData::NAME_DOZE, "doze.msa" },
	{ CRaceMotionData::NAME_SELFIE, "selfie.msa" },
#endif
};

/**
* \brief Emoticons vector.
*/
static const std::vector<TEmoticon> m_vecEmoticons =
{
	{ CInstanceBase::EFFECT_EMOTICON + 0, "", string_path({ EMOTICON_PATH, "sweat.mse" }), "(È²´ç)" },
	{ CInstanceBase::EFFECT_EMOTICON + 1, "", string_path({ EMOTICON_PATH, "money.mse" }), "(µ·)" },
	{ CInstanceBase::EFFECT_EMOTICON + 2, "", string_path({ EMOTICON_PATH, "happy.mse" }), "(±â»Ý)" },
	{ CInstanceBase::EFFECT_EMOTICON + 3, "", string_path({ EMOTICON_PATH, "love_s.mse" }), "(ÁÁ¾Æ)" },
	{ CInstanceBase::EFFECT_EMOTICON + 4, "", string_path({ EMOTICON_PATH, "love_l.mse" }), "(»ç¶û)" },
	{ CInstanceBase::EFFECT_EMOTICON + 5, "", string_path({ EMOTICON_PATH, "angry.mse" }), "(ºÐ³ë)" },
	{ CInstanceBase::EFFECT_EMOTICON + 6, "", string_path({ EMOTICON_PATH, "aha.mse" }), "(¾ÆÇÏ)" },
	{ CInstanceBase::EFFECT_EMOTICON + 7, "", string_path({ EMOTICON_PATH, "gloom.mse" }), "(¿ì¿ï)" },
	{ CInstanceBase::EFFECT_EMOTICON + 8, "", string_path({ EMOTICON_PATH, "sorry.mse" }), "(ÁË¼Û)" },
	{ CInstanceBase::EFFECT_EMOTICON + 9, "", string_path({ EMOTICON_PATH, "!_mix_back.mse" }), "(!)" },
	{ CInstanceBase::EFFECT_EMOTICON + 10, "", string_path({ EMOTICON_PATH, "question.mse" }), "(?)" },
	{ CInstanceBase::EFFECT_EMOTICON + 11, "", string_path({ EMOTICON_PATH, "fish.mse" }), "(fish)" },
#ifdef ENABLE_EXPRESSING_EMOTION
	{ CInstanceBase::EFFECT_EMOTICON + 12, "", string_path({ EMOTICON_PATH, "charging.mse" }), "(charging)" },
	{ CInstanceBase::EFFECT_EMOTICON + 13, "", string_path({ EMOTICON_PATH, "nosay.mse" }), "(nosay)" },
	{ CInstanceBase::EFFECT_EMOTICON + 14, "", string_path({ EMOTICON_PATH, "weather1.mse" }), "(weather1)" },
	{ CInstanceBase::EFFECT_EMOTICON + 15, "", string_path({ EMOTICON_PATH, "weather2.mse" }), "(weather2)" },
	{ CInstanceBase::EFFECT_EMOTICON + 16, "", string_path({ EMOTICON_PATH, "weather3.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 17, "", string_path({ EMOTICON_PATH, "hungry.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 18, "", string_path({ EMOTICON_PATH, "siren.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 19, "", string_path({ EMOTICON_PATH, "letter.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 20, "", string_path({ EMOTICON_PATH, "call.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 21, "", string_path({ EMOTICON_PATH, "celebration.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 22, "", string_path({ EMOTICON_PATH, "alcohol.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 23, "", string_path({ EMOTICON_PATH, "busy.mse" }), "(fish)" },
	{ CInstanceBase::EFFECT_EMOTICON + 24, "", string_path({ EMOTICON_PATH, "whirl.mse" }), "(fish)" },
#endif
};

/**
* \brief Vector of race
*/
static const std::vector<TRaceInfo> m_vecRaceData =
{
	{ "msm/warrior_m.msm", "d:/ymir work/pc/warrior/" },
	{ "msm/assassin_w.msm", "d:/ymir work/pc/assassin/" },
	{ "msm/sura_m.msm", "d:/ymir work/pc/sura/" },
	{ "msm/shaman_w.msm", "d:/ymir work/pc/shaman/" },

	{ "msm/warrior_w.msm", "d:/ymir work/pc2/warrior/" },
	{ "msm/assassin_m.msm", "d:/ymir work/pc2/assassin/" },
	{ "msm/sura_w.msm", "d:/ymir work/pc2/sura/" },
	{ "msm/shaman_m.msm", "d:/ymir work/pc2/shaman/" },
};

/**
* \brief
*/
static const std::vector<TSoundFileName> m_vecUseSoundFileName =
{
	{ CPythonItem::USESOUND_DEFAULT, "sound/ui/drop.wav" },
	{ CPythonItem::USESOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav" },
	{ CPythonItem::USESOUND_ARMOR, "sound/ui/equip_metal_armor.wav" },
	{ CPythonItem::USESOUND_BOW, "sound/ui/equip_bow.wav" },
	{ CPythonItem::USESOUND_WEAPON, "sound/ui/equip_metal_weapon.wav" },
	{ CPythonItem::USESOUND_POTION, "sound/ui/eat_potion.wav" },
	{ CPythonItem::USESOUND_PORTAL, "sound/ui/potal_scroll.wav" },
};

/**
* \brief
*/
static const std::vector<TSoundFileName> m_vecDropSoundFileName =
{
	{ CPythonItem::DROPSOUND_DEFAULT, "sound/ui/drop.wav" },
	{ CPythonItem::DROPSOUND_ACCESSORY, "sound/ui/equip_ring_amulet.wav" },
	{ CPythonItem::DROPSOUND_ARMOR, "sound/ui/equip_metal_armor.wav" },
	{ CPythonItem::DROPSOUND_BOW, "sound/ui/equip_bow.wav" },
	{ CPythonItem::DROPSOUND_WEAPON, "sound/ui/equip_metal_weapon.wav" },
};

/**
* \brief
*/
static const std::vector<TFlyEffects> m_vecFlyEffects =
{
	{ CFlyingManager::FLY_EXP, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_yellow_small2.msf" },
	{ CFlyingManager::FLY_HP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_small.msf" },
	{ CFlyingManager::FLY_HP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_big.msf" },
	{ CFlyingManager::FLY_SP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_warrior_small.msf" },
	{ CFlyingManager::FLY_SP_MEDIUM, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_small.msf" },
	{ CFlyingManager::FLY_SP_BIG, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_blue_big.msf" },
	{ CFlyingManager::FLY_FIREWORK1, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_1.msf" },
	{ CFlyingManager::FLY_FIREWORK2, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_2.msf" },
	{ CFlyingManager::FLY_FIREWORK3, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_3.msf" },
	{ CFlyingManager::FLY_FIREWORK4, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_4.msf" },
	{ CFlyingManager::FLY_FIREWORK5, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_5.msf" },
	{ CFlyingManager::FLY_FIREWORK6, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_6.msf" },
	{ CFlyingManager::FLY_FIREWORK_XMAS, CFlyingManager::INDEX_FLY_TYPE_FIRE_CRACKER, "d:/ymir work/effect/etc/firecracker/firecracker_xmas.msf" },
	{ CFlyingManager::FLY_CHAIN_LIGHTNING, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/shaman/effect/pokroe.msf" },
	{ CFlyingManager::FLY_HP_SMALL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/effect/etc/gathering/ga_piece_red_smallest.msf" },
	{ CFlyingManager::FLY_SKILL_MUYEONG, CFlyingManager::INDEX_FLY_TYPE_AUTO_FIRE, "d:/ymir work/pc/sura/effect/muyeong_fly.msf" },
#ifdef ENABLE_QUIVER_SYSTEM
	{ CFlyingManager::FLY_QUIVER_ATTACK_NORMAL, CFlyingManager::INDEX_FLY_TYPE_NORMAL, "d:/ymir work/pc/assassin/effect/arrow_02.msf" },
#endif
};

/**
 * \brief 
 */
class CPythonPlayerSettingsModule : public CSingleton<CPythonPlayerSettingsModule>
{
public:
	CPythonPlayerSettingsModule();
	virtual ~CPythonPlayerSettingsModule();

	static auto Load() -> void;

protected:
	static auto __RegisterCacheMotionData(CRaceData* pRaceData, const uint16_t wMotionMode, const uint16_t wMotionIndex, const char* c_szFileName, const uint8_t byPercentage = 100) -> void;
	static auto __SetIntroMotions(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __SetGeneralMotions(CRaceData* pRaceData, const uint16_t wMotionMode, const std::string& c_rstrFolderName) -> void;

	static auto __LoadFishing(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadCombo(CRaceData* pRaceData, const uint16_t wMotionMode) -> void;
	static auto __LoadWeaponMotion(CRaceData* pRaceData, WORD wMotionMode, const std::string & c_rstrFolderName) -> void;
	static auto __LoadHorse(CRaceData * pRaceData, const std::string & c_rstrFolderName) -> void;
	static auto __LoadSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGuildSkill(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __RegisterSharedEmotionAnis(CRaceData* pRaceData, uint16_t wMotionMode,
	                                        const std::string& c_rstrFolderName) -> void;
	static auto __LoadEmotions(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGameWarriorEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGameAssassinEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGameSuraEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGameShamanEx(CRaceData* pRaceData, const std::string& c_rstrFolderName) -> void;
	static auto __LoadGameRace(CRaceData * pRaceData, const std::string & c_rstrFolderName) -> void;
	static auto __LoadGameNPC() -> void;
};
