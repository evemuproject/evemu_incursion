/*
	------------------------------------------------------------------------------------
	LICENSE:
	------------------------------------------------------------------------------------
	This file is part of EVEmu: EVE Online Server Emulator
	Copyright 2006 - 2011 The EVEmu Team
	For the latest information visit http://evemu.org
	------------------------------------------------------------------------------------
	This program is free software; you can redistribute it and/or modify it under
	the terms of the GNU Lesser General Public License as published by the Free Software
	Foundation; either version 2 of the License, or (at your option) any later
	version.

	This program is distributed in the hope that it will be useful, but WITHOUT
	ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
	FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License along with
	this program; if not, write to the Free Software Foundation, Inc., 59 Temple
	Place - Suite 330, Boston, MA 02111-1307, USA, or go to
	http://www.gnu.org/copyleft/lesser.txt.
	------------------------------------------------------------------------------------
	Author:		Almamu
*/

#include "EVEServerPCH.h"


PyCallable_Make_InnerDispatcher(RepairService)


class RepairBound
: public PyBoundObject {
public:

	PyCallable_Make_Dispatcher(RepairBound)
	
	RepairBound(PyServiceMgr *mgr, InsuranceDB *db)
	: PyBoundObject(mgr),
	  m_db(db),
	  m_dispatch(new Dispatcher(this))
	{
		_SetCallDispatcher(m_dispatch);
		
		m_strBoundObjectName = "RepairBound";

		PyCallable_REG_CALL(RepairBound, GetDamageReports)
		PyCallable_REG_CALL(RepairBound, DamageModules)
		PyCallable_REG_CALL(RepairBound, RepairItems)

	}
	virtual ~RepairBound() { delete m_dispatch; }
	virtual void Release() {
		//I hate this statement
		delete this;
	}

	PyCallable_DECL_CALL(GetDamageReports)
	PyCallable_DECL_CALL(DamageModules)
	PyCallable_DECL_CALL(RepairItems)

protected:
	InsuranceDB *const m_db;
	Dispatcher *const m_dispatch;
};



RepairService::RepairService(PyServiceMgr *mgr)
: PyService(mgr, "repairSvc"),
  m_dispatch(new Dispatcher(this))
{
	_SetCallDispatcher(m_dispatch);

	PyCallable_REG_CALL(RepairService, UnasembleItems)
}

RepairService::~RepairService() {
	delete m_dispatch;
}



PyBoundObject* RepairService::_CreateBoundObject( Client* c, const PyRep* bind_args )
{
	_log( CLIENT__MESSAGE, "RepairService bind request for:" );
	bind_args->Dump( CLIENT__MESSAGE, "    " );

	return new RepairBound( m_manager, &m_db );
}


PyResult RepairBound::Handle_GetDamageReports( PyCallArgs& call )
{
	sLog.Debug( "RepairBound", "Called GetDamageReports stub." );

	Call_SingleIntList args;

	if( !args.Decode( &call.tuple ) )
	{
		_log(SERVICE__ERROR, "Cant decode args for GetDamageReports" );
		return new PyNone;
	}

	util_Row res;
	util_Rowset quotes;
	PyDict* rep = new PyDict;

	for( size_t i = 0; i < args.ints.size(); i ++ )
	{

		PyList* lines = new PyList;
		quotes.lines = new PyList;
		res.line = new PyList;
		res.header.clear();
		quotes.header.clear();

		res.header.push_back( "playerStanding" );
		res.header.push_back( "serviceCharge" );
		res.header.push_back( "discount" );
		res.header.push_back( "quote" );

		quotes.header.push_back( "itemID" );
		quotes.header.push_back( "typeID" );
		quotes.header.push_back( "groupID" );
		quotes.header.push_back( "damage" );
		quotes.header.push_back( "maxHealth" );
		quotes.header.push_back( "repairable" );
		quotes.header.push_back( "costToRepairOneUnitOfDamage" );

		uint32 maxHealth = 0;
		uint32 costToRepairOneUnitOfDamage = 0;
		std::vector<InventoryItemRef> modules;

		if( m_manager->item_factory.GetItem( args.ints.at( i ) )->categoryID() == EVEDB::invCategories::Ship )
		{

			ShipRef ship = m_manager->item_factory.GetShip( args.ints.at( i ) );

			// Get all fitted modules
			ship->FindByFlagRange( flagLowSlot0, flagHiSlot7, modules );

			lines->AddItemInt( args.ints.at( i ) );
			lines->AddItemInt( ship->typeID() );
			lines->AddItemInt( ship->groupID() );

			if( ship->HasAttribute( AttrDamage ) )
				lines->AddItemInt( ship->GetAttribute( AttrDamage ).get_int() );
			else
				lines->AddItemInt( 0 );

			if( ship->HasAttribute( AttrHp ) )
				maxHealth += ship->GetAttribute( AttrHp ).get_int();

			lines->AddItemInt( maxHealth );
			lines->AddItemInt( 1 ); // The ship is repairable, HACK

			costToRepairOneUnitOfDamage = ship->typeID(); // Hack while i try to guess the formula

			lines->AddItemInt( costToRepairOneUnitOfDamage );

			quotes.lines->AddItem( lines );

			for( size_t i = 0; i < modules.size(); i ++ )
			{
				lines = new PyList;
				lines->AddItemInt( modules.at( i )->itemID() );
				lines->AddItemInt( modules.at( i )->typeID() );
				lines->AddItemInt( modules.at( i )->groupID() );

				if( modules.at( i )->HasAttribute( AttrDamage ) )
					lines->AddItemInt( modules.at( i )->GetAttribute( AttrDamage ).get_int() );
				else
					lines->AddItemInt( 0 );

				if( modules.at( i )->HasAttribute( AttrHp ) )
					lines->AddItemInt( modules.at( i )->GetAttribute( AttrHp ).get_int() );

				lines->AddItemInt( 1 ); // Repairable item. HACK

				lines->AddItemInt( modules.at( i )->typeID() ); // Hack while i try to guess the formula

				quotes.lines->AddItem( lines );
			}
		}
		else
		{
			lines = new PyList;
			InventoryItemRef item = m_manager->item_factory.GetItem( args.ints.at( i ) );

			lines->AddItemInt( item->itemID() );
			lines->AddItemInt( item->typeID() );
			lines->AddItemInt( item->groupID() );
			
			if( item->HasAttribute( AttrDamage ) )
				lines->AddItemInt( item->GetAttribute( AttrDamage ).get_int() );
			else
				lines->AddItemInt( 0 );

			if( item->HasAttribute( AttrHp ) )
				lines->AddItemInt( item->GetAttribute( AttrHp ).get_int() );

			lines->AddItemInt( 1 ); // Repairable item. HACK
			
			lines->AddItemInt( item->typeID() ); // Hack while i try to guess the formula

			quotes.lines->AddItem( lines );
		}

		// Big hacks here, anyway them doesnt hurt anyone :P
		res.line->AddItemReal( 0.0 ); // We should be able to get this from elsewhere
		res.line->AddItemString( "0%" );
		res.line->AddItemString( "100.0%" );
		res.line->AddItem( quotes.Encode() );

		rep->SetItem( new PyInt( args.ints.at( i ) ), res.Encode() );
	}

	return rep;
}


PyResult RepairService::Handle_UnasembleItems( PyCallArgs& call )
{
	Call_UnasembleItems ships;
	
	if( !ships.Decode( &call.tuple ) )
	{
		_log(SERVICE__ERROR, "Cant decode args for UnasembleItems" );
		return new PyNone;
	}

	PyDict::storage_type::iterator cur, end;

	cur = ships.items->items.begin();
	end = ships.items->items.end();

	for(; cur != end; cur++ )
	{
		ShipRef ship = m_manager->item_factory.GetShip( cur->second->AsList()->GetItem( 0 )->AsTuple()->GetItem( 0 )->AsInt()->value() );

		std::vector<InventoryItemRef> items;
		ship->FindByFlag( flagHangar, items );
		ship->FindByFlagRange( flagLowSlot0, flagHiSlot7, items );
		ship->FindByFlagRange( flagRigSlot0, flagRigSlot7, items );
	
		for( size_t i = 0; i < items.size(); i ++ )
		{
			InventoryItemRef item = items.at( i );
			item->Move( call.client->GetStationID(), flagHangar );
		}

		ship->ChangeSingleton( false );
		m_db.UnInsureShip( ship->itemID() );

	}

	return new PyNone;
}


PyResult RepairBound::Handle_DamageModules( PyCallArgs& call )
{
	Call_DamageModules args;

	if( !args.Decode( &call.tuple ) )
	{
		_log(SERVICE__ERROR, "Cant decode args for DamageModules" );
		return new PyNone;
	}

	// Apply the damage percentage 
	// The client directly sends the percentage

	uint32 itemID = 0;
	uint32 damage = 0;

	for( size_t i = 0; i < args.data->size(); i ++ )
	{
		itemID = args.data->GetItem( i )->AsTuple()->GetItem( 0 )->AsInt()->value();
		damage = args.data->GetItem( i )->AsTuple()->GetItem( 1 )->AsInt()->value();	

		InventoryItemRef item = m_manager->item_factory.GetItem( itemID );

		if( !item->HasAttribute( AttrDamage ) )
		{
			_log( SERVICE__ERROR, "Item %u doesnt has attribute %u", itemID, AttrDamage );
			return new PyNone;
		}

		item->SetAttribute( AttrDamage, damage );
	}

	return new PyNone;
}


PyResult RepairBound::Handle_RepairItems( PyCallArgs& call )
{
	Call_RepairItems args;

	if( !args.Decode( &call.tuple ) )
	{
		_log(SERVICE__ERROR, "Cant decode args for RepairItems" );
		return new PyNone;
	}

	double cost = args.cost;
	double total = 0.0;
	double total_paid = 0.0;

	// This will repair the items we can afford
	for( size_t i = 0; i < args.itemID->size(); i ++ )
	{
		InventoryItemRef item = m_manager->item_factory.GetItem( args.itemID->GetItem( i )->AsInt()->value() );
		total = item->typeID() * item->GetAttribute( AttrDamage ).get_int();

		if( cost >= total )
		{
			call.client->AddBalance( -total );
			item->SetAttribute( AttrDamage, 0, true );
			cost -= total;
			total_paid += total;
		}
		else
		{
			continue;
		}
	}

	std::string reason = "Repair cost for ";
	reason += m_manager->item_factory.GetItem( args.itemID->GetItem( 0 )->AsInt()->value() )->itemName();
	reason += ".";

	if( !m_db->GiveCash(
		call.client->GetCharacterID(),
		RefType_RepairBill,
		call.client->GetCharacterID(),
		1, 
		"1",
		call.client->GetAccountID(),
		accountCash,
		-total_paid,
		call.client->GetBalance(),
		reason.c_str ()
		) )
	{
		codelog(CLIENT__ERROR, "Failed to record repair transaction." );
	}

	return new PyNone;
}

