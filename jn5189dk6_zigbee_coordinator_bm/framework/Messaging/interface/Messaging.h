/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2022 NXP
* All rights reserved.
*
* \file
*
* This is a header file for the messaging module.
* It contains the type definitions of inter-task messaging.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _FWK_MESSAGING_H_
#define _FWK_MESSAGING_H_


/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "GenericList.h"
#include "MemManager.h"

#if defined(gMemManagerLight) && (gMemManagerLight != 0)
/*
 * Messaging module uses the underlying generic list module.
 * The LIST_* APIs are redefined for backward compatibility.
 */
#include "fsl_component_messaging.h"

#define ListGetList          LIST_GetList
#define ListAddHead          LIST_AddHead
#define ListAddTail          LIST_AddTail

#define ListRemoveHead      LIST_RemoveHead
#define ListGetHead         LIST_GetHead
#define ListGetNext         LIST_GetNext
#define ListGetPrev         LIST_GetPrev
#define ListRemoveElement   LIST_RemoveElement
#define ListAddPrevElement  LIST_AddPrevElement
#define ListGetSize         LIST_GetSize
#define ListGetAvailable    LIST_GetAvailableSize
#define msgQueue_t          list_label_t
#define anchor_t            list_label_t
/* Put a message in a queue. */
#define MSG_Queue(anchor, element)     MSG_QueueAddTail((anchor), (element))
#define MSG_QueueHead(anchor, element) MSG_QueueAddHead((anchor), (element))

/* Get a message from a queue. Returns NULL if no messages in queue. */
#define MSG_DeQueue(anchor)            MSG_QueueRemoveHead(anchor)

/* Check if a message is pending in a queue. Returns */
/* TRUE if any pending messages, and FALSE otherwise. */
#define MSG_Pending(anchor) ((anchor)->head != NULL)

#define MSG_InitQueue(anchor)       MSG_QueueInit(anchor)
#define List_ClearAnchor(anchor)    MSG_QueueInit(anchor)

#define MSG_GetHead(anchor)        ListGetHeadMsg(anchor)
#define MSG_AllocType(type)        MSG_Alloc(sizeof(type))
#define MM_Free                    MSG_Free
#define MSG_FreeQueue(anchor)      while(MSG_Pending(anchor)) { (void)MSG_Free(MSG_DeQueue(anchor)); }

#else /* gMemManagerLight */
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
#define  anchor_t        list_t
#define  msgQueue_t      list_t

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
/* Put a message in a queue. */
#define MSG_Queue(anchor, element) ListAddTailMsg((anchor), (element))
#define MSG_QueueHead(anchor, element) ListAddHeadMsg((anchor), (element))

/* Get a message from a queue. Returns NULL if no messages in queue. */
#define MSG_DeQueue(anchor) ListRemoveHeadMsg(anchor)

/* Check if a message is pending in a queue. Returns */
/* TRUE if any pending messages, and FALSE otherwise. */
#define MSG_Pending(anchor) ((anchor)->head != NULL)

#define MSG_InitQueue(anchor) ListInitMsg(anchor)
#define List_ClearAnchor(anchor) ListInitMsg(anchor)

#define MSG_GetHead(anchor)         ListGetHeadMsg(anchor)
#define  MSG_AllocType(type)        MSG_Alloc(sizeof(type))
#define  MM_Free                    MSG_Free
#define  MSG_FreeQueue(anchor)      while(MSG_Pending(anchor)) { (void)MSG_Free(MSG_DeQueue(anchor)); }

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

#define ListInitMsg(listPtr) ListInit((listPtr), 0)
listStatus_t ListAddTailMsg    ( listHandle_t list, void* pMsg );
listStatus_t ListAddHeadMsg    ( listHandle_t list, void* pMsg );
listStatus_t ListAddPrevMsg    ( void* pMsg, void* pNewMsg );
listStatus_t ListRemoveMsg     ( void* pMsg );
void *ListRemoveHeadMsg( listHandle_t list );
void *ListGetHeadMsg   ( listHandle_t list );
void *ListGetNextMsg   ( void* pMsg );

/*! *********************************************************************************
* \brief     Allocate a message.
*
* \param[in] msgSize - size to allocate for the message.
*
* \return Pointer to the allocated payload.
*
* \pre
*
* \post
*
* \remarks If using MemManagerLight, BLOCK_HDR_BYTES_OFFSET adds needed overhead
*          for message chaining. If using Legacy MemManager, BLOCK_HDR_BYTES_OFFSET
*          should be 0.
*
********************************************************************************** */
void*       MSG_Alloc   (uint32_t msgSize);

/*! *********************************************************************************
* \brief     Get message's size.
*
* \param[in] buffer - pointer to the message's payload.
*
* \return Message's size.
*
* \pre
*
* \post
*
* \remarks If using MemManagerLight, BLOCK_HDR_BYTES_OFFSET is used to get the right
*          buffer's address. If using Legacy MemManager, BLOCK_HDR_BYTES_OFFSET
*          should be 0.
*
********************************************************************************** */
uint16_t    MSG_GetSize (void* buffer);

/*! *********************************************************************************
* \brief     Free a message's buffer.
*
* \param[in] buffer - pointer to the message's payload.
*
* \return  Memory status after free.
*
* \pre
*
* \post
*
* \remarks If using MemManagerLight, BLOCK_HDR_BYTES_OFFSET is used to get the right
*          buffer's address. If using Legacy MemManager, BLOCK_HDR_BYTES_OFFSET
*          should be 0.
*
********************************************************************************** */
memStatus_t MSG_Free    (void* buffer);

#ifdef __cplusplus
}
#endif
#endif
/*================================================================================================*/

#endif  /* MESSAGING_H */
