#include "StatBoost.h"

void StatBoosterPlayer::OnLogin(Player* player)
{
    if (!sBoostConfigMgr->Enable)
    {
        return;
    }

	if(sBoostConfigMgr->OnLoginEnable)
	{
		ChatHandler(player->GetSession()).SendSysMessage(sBoostConfigMgr->OnLoginMessage);
	}
}

void StatBoosterPlayer::OnLootItem(Player* player, Item* item, uint32 /*count*/, ObjectGuid /*lootguid*/)
{
    if (!sBoostConfigMgr->Enable)
    {
        return;
    }

    if (sBoostConfigMgr->OnLootItemEnable)
    {
        bool result = statBoostMgr.BoostItem(player, item, sBoostConfigMgr->LootItemChance);

        if (result)
        {
            if (sBoostConfigMgr->AnnounceBoostEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage(sBoostConfigMgr->AnnounceLoot);
            }

            if (sBoostConfigMgr->PlaySoundEnable)
            {
                player->PlayDirectSound(sBoostConfigMgr->SoundId);
            }
        }
    }
}

void StatBoosterPlayer::OnQuestRewardItem(Player* player, Item* item, uint32 /*count*/)
{
    if (!sBoostConfigMgr->Enable)
    {
        return;
    }

    if (sBoostConfigMgr->OnQuestRewardItemEnable)
    {
        bool result = statBoostMgr.BoostItem(player, item, sBoostConfigMgr->QuestRewardChance);

        if (result)
        {
            if (sBoostConfigMgr->AnnounceBoostEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage(sBoostConfigMgr->AnnounceQuest);
            }

            if (sBoostConfigMgr->PlaySoundEnable)
            {
                player->PlayDirectSound(sBoostConfigMgr->SoundId);
            }
        }
    }
}

void StatBoosterPlayer::OnCreateItem(Player* player, Item* item, uint32 /*count*/)
{
    if (!sBoostConfigMgr->Enable)
    {
        return;
    }

    if (sBoostConfigMgr->OnCraftItemEnable)
    {
        bool result = statBoostMgr.BoostItem(player, item, sBoostConfigMgr->CraftItemChance);

        if (result)
        {
            if (sBoostConfigMgr->AnnounceBoostEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage(sBoostConfigMgr->AnnounceCraft);
            }

            if (sBoostConfigMgr->PlaySoundEnable)
            {
                player->PlayDirectSound(sBoostConfigMgr->SoundId);
            }
        }
    }
}

void StatBoosterPlayer::OnGroupRollRewardItem(Player* player, Item* item, uint32 /*count*/, RollVote /*voteType*/, Roll* /*roll*/)
{
    if (!sBoostConfigMgr->Enable)
    {
        return;
    }

    if (sBoostConfigMgr->OnLootItemEnable)
    {
        bool result = statBoostMgr.BoostItem(player, item, sBoostConfigMgr->LootItemChance);

        if (result)
        {
            if (sBoostConfigMgr->AnnounceBoostEnable)
            {
                ChatHandler(player->GetSession()).SendSysMessage(sBoostConfigMgr->AnnounceLoot);
            }

            if (sBoostConfigMgr->PlaySoundEnable)
            {
                player->PlayDirectSound(sBoostConfigMgr->SoundId);
            }
        }
    }
}

void StatBoosterWorld::OnAfterConfigLoad(bool /*reload*/)
{
    sBoostConfigMgr->Enable = sConfigMgr->GetOption<bool>("StatBooster.Enable", false);

    //No point loading all of this information if the module is not enabled.
    if (sBoostConfigMgr->Enable)
    {
        sBoostConfigMgr->VerboseEnable = sConfigMgr->GetOption<bool>("StatBooster.VerboseEnable", false);
        sBoostConfigMgr->OnLoginEnable = sConfigMgr->GetOption<bool>("StatBooster.OnLoginEnable", true);
        sBoostConfigMgr->OnLoginMessage = sConfigMgr->GetOption<std::string>("StatBooster.OnLoginMessage", "This server is running the StatBooster module.");

        sBoostConfigMgr->OnLootItemEnable = sConfigMgr->GetOption<bool>("StatBooster.OnLootItemEnable", true);
        sBoostConfigMgr->OnQuestRewardItemEnable = sConfigMgr->GetOption<bool>("StatBooster.OnQuestRewardItemEnable", true);
        sBoostConfigMgr->OnCraftItemEnable = sConfigMgr->GetOption<bool>("StatBooster.OnCraftItemEnable", true);

        sBoostConfigMgr->LootItemChance = sConfigMgr->GetOption<uint32>("StatBooster.LootItemChance", 100);
        sBoostConfigMgr->QuestRewardChance = sConfigMgr->GetOption<uint32>("StatBooster.QuestRewardChance", 100);
        sBoostConfigMgr->CraftItemChance = sConfigMgr->GetOption<uint32>("StatBooster.CraftItemChance", 100);

        sBoostConfigMgr->MinQuality = sConfigMgr->GetOption<uint32>("StatBooster.MinQuality", ITEM_QUALITY_UNCOMMON);
        sBoostConfigMgr->MaxQuality = sConfigMgr->GetOption<uint32>("StatBooster.MaxQuality", ITEM_QUALITY_EPIC);

        sBoostConfigMgr->PlaySoundEnable = sConfigMgr->GetOption<bool>("StatBooster.PlaySoundEnable", true);
        sBoostConfigMgr->SoundId = sConfigMgr->GetOption<uint32>("StatBooster.SoundId", 120);

        sBoostConfigMgr->AnnounceBoostEnable = sConfigMgr->GetOption<bool>("StatBooster.AnnounceBoostEnable", true);
        sBoostConfigMgr->AnnounceLoot = sConfigMgr->GetOption<std::string>("StatBooster.AnnounceLoot", "You looted a boosted item.");
        sBoostConfigMgr->AnnounceQuest = sConfigMgr->GetOption<std::string>("StatBooster.AnnounceQuest", "You received a boosted item.");
        sBoostConfigMgr->AnnounceCraft = sConfigMgr->GetOption<std::string>("StatBooster.AnnounceCraft", "You crafted a boosted item.");

        sBoostConfigMgr->OverwriteEnchantEnable = sConfigMgr->GetOption<bool>("StatBooster.OverwriteEnchantEnable", true);

        sBoostConfigMgr->EnchantPool.Load();
    }
}

ChatCommandTable StatBoosterCommands::GetCommands() const
{
    static ChatCommandTable sbConfCommandTable =
    {
        { "set", HandleSBConfSetCommand, SEC_GAMEMASTER, Console::Yes }
    };

    static ChatCommandTable sbCommandTable =
    {
        { "reload", HandleSBReloadCommand, SEC_GAMEMASTER, Console::Yes  },
        { "additem", HandleSBAddItemCommand, SEC_GAMEMASTER, Console::No },
        { "config", sbConfCommandTable }
    };

    static ChatCommandTable commandTable =
    {
        { "sb", sbCommandTable }
    };

    return commandTable;
}

bool StatBoosterCommands::HandleSBReloadCommand(ChatHandler* handler)
{
    handler->SendSysMessage("Reloading `statbooster_enchant_template`..");
    sBoostConfigMgr->EnchantPool.Load();
    handler->SendSysMessage("Reload complete.");
    return true;
}

bool StatBoosterCommands::HandleSBAddItemCommand(ChatHandler* handler, uint32 itemId, uint32 count)
{
    if (!itemId || !count)
    {
        handler->SendSysMessage("Invalid arguments, you must supply a valid itemId and count.");
        handler->SendSysMessage("Ex: '.sb additem <itemId> <count>'");
        return false;
    }

    ItemTemplate const* itemTemp = sObjectMgr->GetItemTemplate(itemId);

    if (!itemTemp)
    {
        handler->SendSysMessage("Item template could not be found. Is this a valid item id?");
        return false;
    }

    Player* player = handler->GetPlayer();

    if (!player)
    {
        return false;
    }

    if (player->GetTarget() && player->GetTarget().IsPlayer())
    {
        player = ObjectAccessor::FindPlayer(player->GetTarget());
    }

    uint32 noSpaceForCount = 0;
    ItemPosCountVec dest;
    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, itemId, count, &noSpaceForCount);
    
    if (msg != EQUIP_ERR_OK)
    {
        count -= noSpaceForCount;
    }

    if (dest.empty())
    {
        return false;
    }

    Item* item = player->StoreNewItem(dest, itemId, true);

    StatBoostMgr statBoostMgr;
    bool result = statBoostMgr.BoostItem(player, item, 100);

    if (!item || !result)
    {
        return false;
    }

    handler->SendSysMessage(Acore::StringFormatFmt("Added boosted item '{}' to '{}'.", itemId, player->GetPlayerName()));
    player->SendNewItem(item, count, true, false, false, true);

    return true;
}

bool StatBoosterCommands::HandleSBConfSetCommand(ChatHandler* handler, const char* args...)
{
    
}

void AddSCStatBoosterScripts()
{
    new StatBoosterCommands();
    new StatBoosterWorld();
    new StatBoosterPlayer();
}
