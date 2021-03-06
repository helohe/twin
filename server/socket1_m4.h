/* Copyright (C) 2000 by Massimiliano Ghilardi
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */


/* This file was automatically generated from m4/socket1.m4, do not edit! */

/* clang-format off */












































case order_FindFunction:
    if (a.size() >= 4)
        a[0]_any = (tany)sockFindFunction((byte)a[1]_any, (const char *)a[2]_vec, (byte)a[3]_any, (const char *)a[4]_vec);
    break;



case order_SyncSocket:
    if (a.size() >= 0)
        a[0]_any = (tany)sockSyncSocket();
    break;



case order_ServerSizeof:
    if (a.size() >= 1)
        a[0]_any = (tany)sockServerSizeof((byte)a[1]_any);
    break;



case order_CanCompress:
    if (a.size() >= 0)
        a[0]_any = (tany)sockCanCompress();
    break;


case order_DoCompress:
    if (a.size() >= 1)
        a[0]_any = (tany)sockDoCompress((byte)a[1]_any);
    break;



case order_NeedResizeDisplay:
    if (a.size() >= 0)
        sockNeedResizeDisplay();
    break;



case order_AttachHW:
    if (a.size() >= 3)
        sockAttachHW((uldat)a[1]_any, (const char *)a[2]_vec, (byte)a[3]_any);
    break;


case order_DetachHW:
    if (a.size() >= 2)
        a[0]_any = (tany)sockDetachHW((uldat)a[1]_any, (const char *)a[2]_vec);
    break;



case order_SetFontTranslation:
    if (a.size() >= 1)
        sockSetFontTranslation((const byte *)a[1]_vec);
    break;


case order_SetTRuneTranslation:
    if (a.size() >= 1)
        sockSetTRuneTranslation((const trune *)a[1]_vec);
    break;



case order_DeleteObj:
    if (a.size() >= 1)
        sockDeleteObj((obj)a[1]_obj);
    break;


case order_ChangeFieldObj:
    if (a.size() >= 4)
        Act(ChangeField,(obj)a[1]_obj)((obj)a[1]_obj, (udat)a[2]_any, (uldat)a[3]_any, (uldat)a[4]_any);
    break;



case order_CreateWidget:
    if (a.size() >= 7)
        a[0]_obj = (obj)sockCreateWidget((dat)a[1]_any, (dat)a[2]_any, (uldat)a[3]_any, (uldat)a[4]_any, (dat)a[5]_any, (dat)a[6]_any, (tcell)a[7]_any);
    break;


case order_RecursiveDeleteWidget:
    if (a.size() >= 1)
        sockRecursiveDeleteWidget((widget)a[1]_obj);
    break;

case order_MapWidget:
    if (a.size() >= 2)
        Act(Map,(widget)a[1]_obj)((widget)a[1]_obj, (widget)a[2]_obj);
    break;

case order_UnMapWidget:
    if (a.size() >= 1)
        Act(UnMap,(widget)a[1]_obj)((widget)a[1]_obj);
    break;

case order_SetXYWidget:
    if (a.size() >= 3)
        sockSetXYWidget((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any);
    break;

case order_ResizeWidget:
    if (a.size() >= 3)
        sockResizeWidget((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any);
    break;

case order_ScrollWidget:
    if (a.size() >= 3)
        sockScrollWidget((widget)a[1]_obj, (ldat)a[2]_any, (ldat)a[3]_any);
    break;

case order_DrawWidget:
    if (a.size() >= 8)
        sockDrawWidget((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any, (dat)a[4]_any, (dat)a[5]_any, (const char *)a[6]_vec, (const trune *)a[7]_vec, (const tcell *)a[8]_vec);
    break;


case order_SetVisibleWidget:
    if (a.size() >= 2)
        sockSetVisibleWidget((widget)a[1]_obj, (byte)a[2]_any);
    break;


case order_FocusSubWidget:
    if (a.size() >= 1)
        sockFocusSubWidget((widget)a[1]_obj);
    break;

case order_FindWidgetAtWidget:
    if (a.size() >= 3)
        a[0]_obj = (obj)Act(FindWidgetAt,(widget)a[1]_obj)((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any);
    break;


case order_RaiseWidget:
    if (a.size() >= 1)
        Act(Raise,(widget)a[1]_obj)((widget)a[1]_obj);
    break;

case order_LowerWidget:
    if (a.size() >= 1)
        Act(Lower,(widget)a[1]_obj)((widget)a[1]_obj);
    break;

case order_RestackChildrenWidget:
    if (a.size() >= 3)
        sockRestackChildrenWidget((widget)a[1]_obj, (uldat)a[2]_any, (const widget *)a[3]_vec);
    break;

case order_CirculateChildrenWidget:
    if (a.size() >= 2)
        sockCirculateChildrenWidget((widget)a[1]_obj, (byte)a[2]_any);
    break;



case order_CreateGadget:
    if (a.size() >= 13)
        a[0]_obj = (obj)sockCreateGadget((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any, (const char *)a[4]_vec, (uldat)a[5]_any, (uldat)a[6]_any, (udat)a[7]_any, (tcolor)a[8]_any, (tcolor)a[9]_any, (tcolor)a[10]_any, (tcolor)a[11]_any, (dat)a[12]_any, (dat)a[13]_any);
    break;


case order_CreateButtonGadget:
    if (a.size() >= 11)
        a[0]_obj = (obj)Do(CreateButton,gadget)((widget)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any, (const char *)a[4]_vec, (uldat)a[5]_any, (udat)a[6]_any, (tcolor)a[7]_any, (tcolor)a[8]_any, (tcolor)a[9]_any, (dat)a[10]_any, (dat)a[11]_any);
    break;


case order_WriteTextsGadget:
    if (a.size() >= 7)
        Act(WriteTexts,(gadget)a[1]_obj)((gadget)a[1]_obj, (byte)a[2]_any, (dat)a[3]_any, (dat)a[4]_any, (const char *)a[5]_vec, (dat)a[6]_any, (dat)a[7]_any);
    break;

case order_WriteTRunesGadget:
    if (a.size() >= 7)
        Act(WriteTRunes,(gadget)a[1]_obj)((gadget)a[1]_obj, (byte)a[2]_any, (dat)a[3]_any, (dat)a[4]_any, (const trune *)a[5]_vec, (dat)a[6]_any, (dat)a[7]_any);
    break;


case order_CreateWindow:
    if (a.size() >= 11)
        a[0]_obj = (obj)sockCreateWindow((dat)a[1]_any, (const char *)a[2]_vec, (const tcolor *)a[3]_vec, (menu)a[4]_obj, (tcolor)a[5]_any, (uldat)a[6]_any, (uldat)a[7]_any, (uldat)a[8]_any, (dat)a[9]_any, (dat)a[10]_any, (dat)a[11]_any);
    break;

case order_Create4MenuWindow:
    if (a.size() >= 1)
        a[0]_obj = (obj)Do(Create4Menu,window)((menu)a[1]_obj);
    break;


case order_WriteCharsetWindow:
    if (a.size() >= 3)
        sockWriteCharsetWindow((window)a[1]_obj, (uldat)a[2]_any, (const char *)a[3]_vec);
    break;

case order_WriteUtf8Window:
    if (a.size() >= 3)
        sockWriteUtf8Window((window)a[1]_obj, (uldat)a[2]_any, (const char *)a[3]_vec);
    break;

case order_WriteTRuneWindow:
    if (a.size() >= 3)
        sockWriteTRuneWindow((window)a[1]_obj, (uldat)a[2]_any, (const trune *)a[3]_vec);
    break;

case order_WriteTCellWindow:
    if (a.size() >= 5)
        sockWriteTCellWindow((window)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any, (uldat)a[4]_any, (const tcell *)a[5]_vec);
    break;


case order_GotoXYWindow:
    if (a.size() >= 3)
        Act(GotoXY,(window)a[1]_obj)((window)a[1]_obj, (ldat)a[2]_any, (ldat)a[3]_any);
    break;

case order_SetTitleWindow:
    if (a.size() >= 3)
        sockSetTitleWindow((window)a[1]_obj, (dat)a[2]_any, (const char *)a[3]_vec);
    break;

case order_SetColTextWindow:
    if (a.size() >= 2)
        Act(SetColText,(window)a[1]_obj)((window)a[1]_obj, (tcolor)a[2]_any);
    break;

case order_SetColorsWindow:
    if (a.size() >= 11)
        Act(SetColors,(window)a[1]_obj)((window)a[1]_obj, (udat)a[2]_any, (tcolor)a[3]_any, (tcolor)a[4]_any, (tcolor)a[5]_any, (tcolor)a[6]_any, (tcolor)a[7]_any, (tcolor)a[8]_any, (tcolor)a[9]_any, (tcolor)a[10]_any, (tcolor)a[11]_any);
    break;

case order_ConfigureWindow:
    if (a.size() >= 8)
        Act(Configure,(window)a[1]_obj)((window)a[1]_obj, (byte)a[2]_any, (dat)a[3]_any, (dat)a[4]_any, (dat)a[5]_any, (dat)a[6]_any, (dat)a[7]_any, (dat)a[8]_any);
    break;

case order_FindRowByCodeWindow:
    if (a.size() >= 2)
        a[0]_obj = (obj)sockFindRowByCodeWindow((window)a[1]_obj, (dat)a[2]_any);
    break;


case order_CreateGroup:
    if (a.size() >= 0)
        a[0]_obj = (obj)sockCreateGroup();
    break;

case order_InsertGadgetGroup:
    if (a.size() >= 2)
        Act(InsertGadget,(ggroup)a[1]_obj)((ggroup)a[1]_obj, (gadget)a[2]_obj);
    break;

case order_RemoveGadgetGroup:
    if (a.size() >= 2)
        Act(RemoveGadget,(ggroup)a[1]_obj)((ggroup)a[1]_obj, (gadget)a[2]_obj);
    break;


case order_GetSelectedGadgetGroup:
    if (a.size() >= 1)
        a[0]_obj = (obj)Act(GetSelectedGadget,(ggroup)a[1]_obj)((ggroup)a[1]_obj);
    break;

case order_SetSelectedGadgetGroup:
    if (a.size() >= 2)
        Act(SetSelectedGadget,(ggroup)a[1]_obj)((ggroup)a[1]_obj, (gadget)a[2]_obj);
    break;


case order_RaiseRow:
    if (a.size() >= 1)
        Act(Raise,(row)a[1]_obj)((row)a[1]_obj);
    break;

case order_LowerRow:
    if (a.size() >= 1)
        Act(Lower,(row)a[1]_obj)((row)a[1]_obj);
    break;

case order_RestackChildrenRow:
    if (a.size() >= 3)
        sockRestackChildrenRow((obj)a[1]_obj, (uldat)a[2]_any, (const row *)a[3]_vec);
    break;

case order_CirculateChildrenRow:
    if (a.size() >= 2)
        sockCirculateChildrenRow((obj)a[1]_obj, (byte)a[2]_any);
    break;


case order_Create4MenuAny:
    if (a.size() >= 6)
        a[0]_obj = (obj)sockCreate4MenuAny((obj)a[1]_obj, (window)a[2]_obj, (udat)a[3]_any, (byte)a[4]_any, (ldat)a[5]_any, (const char *)a[6]_vec);
    break;


case order_Create4MenuCommonMenuItem:
    if (a.size() >= 1)
        a[0]_any = (tany)Do(Create4MenuCommon,menuitem)((menu)a[1]_obj);
    break;


case order_CreateMenu:
    if (a.size() >= 7)
        a[0]_obj = (obj)sockCreateMenu((tcolor)a[1]_any, (tcolor)a[2]_any, (tcolor)a[3]_any, (tcolor)a[4]_any, (tcolor)a[5]_any, (tcolor)a[6]_any, (byte)a[7]_any);
    break;

case order_SetInfoMenu:
    if (a.size() >= 5)
        Act(SetInfo,(menu)a[1]_obj)((menu)a[1]_obj, (byte)a[2]_any, (ldat)a[3]_any, (const char *)a[4]_vec, (const tcolor *)a[5]_vec);
    break;


case order_CreateMsgPort:
    if (a.size() >= 2)
        a[0]_obj = (obj)sockCreateMsgPort((byte)a[1]_any, (const char *)a[2]_vec);
    break;

case order_FindMsgPort:
    if (a.size() >= 3)
        a[0]_obj = (obj)sockFindMsgPort((msgport)a[1]_obj, (byte)a[2]_any, (const char *)a[3]_vec);
    break;


case order_BgImageScreen:
    if (a.size() >= 4)
        Act(BgImage,(screen)a[1]_obj)((screen)a[1]_obj, (dat)a[2]_any, (dat)a[3]_any, (const tcell *)a[4]_vec);
    break;


case order_PrevObj:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockPrevObj((obj)a[1]_obj);
    break;

case order_NextObj:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockNextObj((obj)a[1]_obj);
    break;

case order_ParentObj:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockParentObj((obj)a[1]_obj);
    break;


case order_FirstScreen:
    if (a.size() >= 0)
        a[0]_obj = (obj)sockFirstScreen();
    break;

case order_FirstWidget:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstWidget((widget )a[1]_obj);
    break;

case order_FirstMsgPort:
    if (a.size() >= 0)
        a[0]_obj = (obj)sockFirstMsgPort();
    break;

case order_FirstMenu:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstMenu((msgport)a[1]_obj);
    break;

case order_FirstW:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstW((msgport)a[1]_obj);
    break;

case order_FirstGroup:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstGroup((msgport)a[1]_obj);
    break;

case order_FirstMutex:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstMutex((msgport)a[1]_obj);
    break;

case order_FirstMenuItem:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstMenuItem((menu   )a[1]_obj);
    break;

case order_FirstGadget:
    if (a.size() >= 1)
        a[0]_obj = (obj)sockFirstGadget((ggroup  )a[1]_obj);
    break;


case order_GetDisplayWidth:
    if (a.size() >= 0)
        a[0]_any = (tany)sockGetDisplayWidth();
    break;

case order_GetDisplayHeight:
    if (a.size() >= 0)
        a[0]_any = (tany)sockGetDisplayHeight();
    break;

case order_GetAll:
    if (a.size() >= 0)
        a[0]_obj = (obj)sockGetAll();
    break;


case order_SendToMsgPort:
    if (a.size() >= 3)
        a[0]_any = (tany)sockSendToMsgPort((msgport)a[1]_obj, (udat)a[2]_any, (const byte *)a[3]_vec);
    break;

case order_BlindSendToMsgPort:
    if (a.size() >= 3)
        sockBlindSendToMsgPort((msgport)a[1]_obj, (udat)a[2]_any, (const byte *)a[3]_vec);
    break;


case order_GetOwnerSelection:
    if (a.size() >= 0)
        a[0]_obj = (obj)sockGetOwnerSelection();
    break;

case order_SetOwnerSelection:
    if (a.size() >= 2)
        sockSetOwnerSelection((tany)a[1]_any, (tany)a[2]_any);
    break;

case order_RequestSelection:
    if (a.size() >= 2)
        sockRequestSelection((obj)a[1]_obj, (uldat)a[2]_any);
    break;

case order_NotifySelection:
    if (a.size() >= 6)
        sockNotifySelection((obj)a[1]_obj, (uldat)a[2]_any, (uldat)a[3]_any, (const char *)a[4]_vec, (uldat)a[5]_any, (const char *)a[6]_vec);
    break;


case order_SetServerUid:
    if (a.size() >= 2)
        a[0]_any = (tany)sockSetServerUid((uldat)a[1]_any, (byte)a[2]_any);
    break;


