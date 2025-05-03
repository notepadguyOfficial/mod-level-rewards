#include "ScriptMgr.h"
#include "Player.h"
#include "Mail.h"
#include "ObjectAccessor.h"
#include "DatabaseEnv.h"

class LevelRewardPlayerScript : PlayerScript
{
public:
	LevelRewardPlayerScript() : PlayerScript("LevelRewardPlayerScript") {}
	
	void OnPlayerLevelChanged(Player* player, uint8 level) override
	{
		if(!player || player->GetSession()->HasPermission(rabc::RBAC_PERM_SKIP_CHECKS))
			return;

		uint32 Sender = 10667;
		uint8 Stationary = MAIL_STATIONERY_GM;
		std::string subject = "Blizzards Entertainment Rewards";
        std::string body = "!\n\nYou've done well while advancing. Here is a small reward to celebrate your heroic deeds. Go forth!\n\nKind regards,\nBlizzards Team.";
		
		std::map<uint8, uint32> Coins = {
            {5, 50000}, {10, 100000}, {15, 150000}, {20, 200000},
            {25, 250000}, {30, 300000}, {35, 350000}, {40, 400000},
            {45, 450000}, {50, 500000}, {55, 550000}, {60, 600000},
            {65, 650000}, {70, 700000}, {75, 750000}, {80, 800000},
            {85, 850000}
        };
		
		 std::map<uint8, std::vector<std::pair<uint32, uint32>>> Items = {
            {5, {{51809, 4}}},
            {10, {{49313, 2}}}
        };
		
		if(Items.count(level + 1))
		{
			MailDraft draft(subject, "Hello " + player->GetName() + body);
			draft.SetStationary(static_cast<MailStationary>(Stationary));
			
			if(Coins.count(level + 1))
				draft.AddMoney(Coins[level + 1]);
			
			for(auto const& Data : Items[level + 1])
			{
				if(Item* item = Item::CreateItem(Data.first, Data.second, player))
					draft.AddItem(item);
			}
			
			draft.SendMailTo(player, MailSender(MAIL_CREATURE, Sender), MAIL_CHECK_MASK_NONE);
		}
		
		else if(Coins.count(level + 1)
			player->ModifyMoney(Coins[level + 1]);
		
		CharacterDatabase.Execute("UPDATE mail SET messageType = 3 WHERE sender = {} AND receiver = {} AND messageType = 0", Sender, player->GetGUID().GetCounter());
	}
}

void AddLevelReward()
{
	new LevelRewardPlayerScript();
}