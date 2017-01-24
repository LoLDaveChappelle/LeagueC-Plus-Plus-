// Lord's Ahri.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PluginSDK.h"

 
 

PluginSetup("Lord's Ahri - LordZEDith");

IMenu* MainMenu;
IMenu* QMenu;
IMenu* WMenu;
IMenu* EMenu;
IMenu* RMenu;
IMenu* Misc;
IMenu* Drawings;
IMenuOption* ComboQ;
IMenuOption* FarmQ;
IMenuOption* ComboW;
IMenuOption* FarmW;
IMenuOption* ComboE;
IMenuOption* ComboR;
IMenuOption* ComboREnemies;
IMenuOption* DrawReady;
IMenuOption* DrawQ;
IMenuOption* DrawW;
IMenuOption* DrawE;

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
	FarmQ =  QMenu->CheckBox("Use Q Farm", true);

	ComboW = WMenu->CheckBox("Use W", true);
	FarmW =  WMenu->CheckBox("Use W Farm", true);

	ComboE = EMenu->CheckBox("Use E", true);
	 	 
	ComboR = RMenu->CheckBox("Auto R when enemies killable", true);
	ComboREnemies = RMenu->AddInteger("Enemies Health %",0,100,20);
	DrawReady = Drawings->CheckBox("Draw Only Ready Spells", true);

	DrawQ = Drawings->CheckBox("Draw Q", true);
	DrawW = Drawings->CheckBox("Draw W", true);
	DrawE = Drawings->CheckBox("Draw E", true);
}
void LoadSpells()
{
	Q = GPluginSDK->CreateSpell2(kSlotQ, kLineCast, false, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
	W = GPluginSDK->CreateSpell2(kSlotW, kLineCast, false, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
	E = GPluginSDK->CreateSpell2(kSlotE, kLineCast, true, false,  static_cast<eCollisionFlags>(kCollidesWithMinions | kCollidesWithYasuoWall));
	R = GPluginSDK->CreateSpell2(kSlotR, kLineCast, false, false, static_cast<eCollisionFlags>(kCollidesWithYasuoWall));
	Q->SetOverrideRange(870);
	W->SetOverrideRange(670);
	E->SetOverrideRange(950);
	R->SetOverrideRange(600);
	Q->SetOverrideDelay(0.25f);
	E->SetOverrideDelay(0.25f);
	Q->SetOverrideRadius(90);
	E->SetOverrideRadius(60);
	Q->SetOverrideSpeed(1550);	
	E->SetOverrideSpeed(1550);
}
void Combo()
{
	if (ComboQ->Enabled())
	{
		if (Q->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, Q->Range());
			Q->CastOnTarget(target, kHitChanceHigh);
		}
	}
	if (ComboW->Enabled())
	{
		if (W->IsReady())
		{
			auto target = GTargetSelector->FindTarget(QuickestKill, SpellDamage, W->Range());
			W->CastOnTarget(target, kHitChanceHigh);
		}
	}
	if (ComboE->Enabled())
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
			if (target->HealthPercent() <= ComboREnemies->GetInteger())
			{
				R->CastOnTarget(target, kHitChanceHigh);
			}
		}
	}
}
void Farm()
{
	if (FarmQ->Enabled())
	{
		if (Q->IsReady())
	    {
		    Q->LastHitMinion();
	    }
	}
	if (FarmW->Enabled())
	{
		if (W->IsReady())
		{
			W->LastHitMinion();
		}
	}

}
PLUGIN_EVENT(void) OnRender()
{
	if (DrawReady->Enabled())
	{
		if (Q->IsReady() && DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (E->IsReady() && DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

		if (W->IsReady() && DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }

	}
	else
	{
		if (DrawQ->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), Q->Range()); }

		if (DrawE->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), E->Range()); }

		if (DrawW->Enabled()) { GRender->DrawOutlinedCircle(GEntityList->Player()->GetPosition(), Vec4(255, 255, 0, 255), W->Range()); }
	}
}
PLUGIN_EVENT(void) OnGameUpdate()
{
	if (GOrbwalking->GetOrbwalkingMode() == kModeCombo)
	{
		Combo();
	}
	
		if (GOrbwalking->GetOrbwalkingMode() == kModeLaneClear)
		{
			Farm();
		}
}

PLUGIN_API void OnLoad(IPluginSDK* PluginSDK)
{

	PluginSDKSetup(PluginSDK);
	Menu();
	LoadSpells();


	GEventManager->AddEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->AddEventHandler(kEventOnRender, OnRender);

}

PLUGIN_API void OnUnload()
{
	MainMenu->Remove();


	GEventManager->RemoveEventHandler(kEventOnGameUpdate, OnGameUpdate);
	GEventManager->RemoveEventHandler(kEventOnRender, OnRender);

}





 

























