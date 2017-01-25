// Lords Sona.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PluginSDK.h"
#include "Color.h"

PluginSetup("Lords Sona - LordZEDith");

IMenu* MainMenu;
IMenu* QMenu;
IMenu* WMenu;
IMenu* EMenu;
IMenu* RMenu;
IMenu* Misc;
IMenu* Drawings;
IMenuOption* ComboQ;
IMenuOption* FarmQ;
IMenuOption* HealthPercent;
IMenuOption* HarassMana;
IMenuOption* HarassManaQ;
IMenuOption* ComboW;
IMenuOption* FarmW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* UltEnemies;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;
IMenuOption* DrawR;

ISpell2* Q;
ISpell2* W;
ISpell2* E;
ISpell2* R;

void  Menu()
{
	MainMenu = GPluginSDK->AddMenu("Lord's Ahri");
	QMenu = MainMenu->AddMenu("Q Settings");
	WMenu = MainMenu->AddMenu("W Settings");
	EMenu = MainMenu->AddMenu("E Settings");
	RMenu = MainMenu->AddMenu("R Settings");
	Drawings = MainMenu->AddMenu("Drawings");

	ComboQ = QMenu->CheckBox("Use Q", true);
	HarassManaQ = QMenu->AddInteger("Mana Manager(%)", 1, 100, 60);
	//FarmQ = QMenu->CheckBox("Use Q Farm", true);

	ComboW = WMenu->CheckBox("Use W", true);
	HealthPercent = WMenu->AddInteger("Health Manager(%)", 1, 100, 60);

	ComboE = EMenu->CheckBox("Use E", true);
	HarassMana = EMenu->AddInteger("Mana Manager(%)", 1, 100, 60);

	ComboR = RMenu->CheckBox("R When X Enemies in Range ", true);
	UltEnemies = RMenu->AddInteger("Minimum enemies to hit with R", 1, 5, 2);
	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);

	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", true);
	DrawE = Drawings->CheckBox("Draw E", true);
	DrawR = Drawings->CheckBox("Draw R", true);
}
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kConeCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
	W = GPluginSDK->CreateSpell2(kSlotW, kConeCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
	E = GPluginSDK->CreateSpell2(kSlotE, kConeCast, true, false, static_cast<eCollisionFlags>(kCollidesWithNothing ));
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, false, static_cast<eCollisionFlags>(kCollidesWithNothing));
	Q->SetOverrideRange(850);
	W->SetOverrideRange(1000);
	E->SetOverrideRange(350);
	R->SetOverrideRange(1000);
	R->SetOverrideDelay(0.5f);
	Q->SetOverrideRadius(125);
	R->SetOverrideSpeed(3000);
	
}
int GetEnemiesInRange(float range)
{
	auto enemies = GEntityList->GetAllHeros(false, true);
	auto enemiesInRange = 0;

	for (auto enemy : enemies)
	{
		if (enemy != nullptr && enemy->GetTeam() != GEntityList->Player()->GetTeam())
		{
			auto flDistance = (enemy->GetPosition() - GEntityList->Player()->GetPosition()).Length();
			if (flDistance < range)
			{
				E->CastOnPlayer();
			}
		}
	}
	return true;
}
void Combo()
{
	if (ComboQ->Enabled()&& (GEntityList->Player()->ManaPercent() >= HarassManaQ->GetInteger()))
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			Q->CastOnTarget(target, kHitChanceHigh);
		}
	}
	if (ComboE->Enabled()&& (GEntityList->Player()->ManaPercent() >= HarassMana->GetInteger())&& GetEnemiesInRange(850))
	{
		if (E->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, E->Range());
			E->CastOnTarget(target, kHitChanceHigh);
		}
	}
	if (ComboR->Enabled())
	{
		if (R->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, R->Range());
			
			
			int enemies = 0;
			Vec3 pos = Vec3();
			R->FindBestCastPosition(false, true, pos, enemies);
			if (enemies >= UltEnemies->GetInteger())
				R->CastOnPosition(pos);
			
		}
	}
}
void AutoW()
{
	if (GEntityList->Player()->HealthPercent() <= HealthPercent->GetInteger()&& ComboW->Enabled())
	{
		W->CastOnPlayer();
	}
}

	
	PLUGIN_EVENT(void) OnRender()
	{
		if (DrawReady->Enabled())
		{
			if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

			if (R->IsReady() && DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }

		}
		else
		{
			if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

			if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

			if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

			if (DrawR->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), R->Range()); }
		}
	}
	PLUGIN_EVENT(void) OnGameUpdate()
	{
		if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
		{
			Combo();
		}
		AutoW();
		
	}

	PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
	{

		PluginSDKSetup(PluginSDK);
		Menu();
		LoadSpells();


		GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->AddEventHandler(kEventOnRender, OnRender);
		GRender->NotificationEx(Color::LightBlue2().Get(), 2, true, true, "Lord's Sona Loaded - V1.1(Small Fixes)");
	}

	PLUGIN_API void OnUnload()
	{
		MainMenu->Remove();


		GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
		GEventManager->RemoveEventHandler(kEventOnRender, OnRender);

	}



