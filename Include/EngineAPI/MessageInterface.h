/////////////////////////////////////////////////////////////////////
// File    : MessageInterface.h
// Desc    : 
// Created : Tuesday, February 20, 2001
// Author  : 
// 
// (c) 2001 Relic Entertainment Inc.
//
#pragma once

/////////////////////////////////////////////////////////////////////
// MessageInterface

// * allows the mod to send UI message/command to other player's ui

// * messages sent through this interface are not sync'ed to the simulation

// * message received are queued until retrieved

// * even though the message will always be sent guaranteed, a reply
// * is not guaranteed: 
// * -not every player will have an interface (AI)
// * -messages are not saved/restored

// * this interface is bound to the local player. if it changes, all queued
// * messages will be discarded

class MessageInterface
{
// interface
public:
	// message size must be < 1024 bytes
	virtual bool	MessageSend
						( 
						const unsigned char*	message,
						const size_t			messageSize,
						const unsigned long*	playerDestArray,
						const size_t			playerDestSize
						) = 0;

	virtual bool	MessageRetrieve( std::vector<unsigned char>& msg, unsigned long& sender ) = 0;
};
