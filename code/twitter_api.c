extern twc_call_result
twc_Get_Account_Settings(twc_state* Twitter,
                         twc_get_account_settingsr_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_ACCOUNT_SETTINGS, ParamList);
}

extern twc_call_result
twc_Account_VerifyCredentials(twc_state* Twitter,
                              twc_account_verifycredentialse_params Params)
{
    twc_key_value_list ParamList = NULL;
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
 
    twc_key_value_pair SkipStatus_KV; 
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValue("skip_status", TWC_BOOLSTR(Params.SkipStatus.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
 
    twc_key_value_pair IncludeEmail_KV; 
    if (Params.IncludeEmail.Exists) {
        IncludeEmail_KV = twc_KeyValue("include_email", TWC_BOOLSTR(Params.IncludeEmail.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEmail_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_ACCOUNT_VERIFYCREDENTIALS, ParamList);
}

extern twc_call_result
twc_Application_RateLimitStatus(twc_state* Twitter,
                                twc_application_ratelimitstatusn_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Resources_KV;
    if (Params.Resources.Exists) {
        Resources_KV = twc_KeyValueStr("resources", Params.Resources.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Resources_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_APPLICATION_RATELIMITSTATUS, ParamList);
}

extern twc_call_result
twc_Blocks_Ids(twc_state* Twitter,
               twc_blocks_idsx_params Params)
{
    twc_key_value_list ParamList = NULL;
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_BLOCKS_IDS, ParamList);
}

extern twc_call_result
twc_Blocks_List(twc_state* Twitter,
                twc_blocks_listdd_params Params)
{
    twc_key_value_list ParamList = NULL;
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_BLOCKS_LIST, ParamList);
}

extern twc_call_result
twc_Collections_Entries(twc_state* Twitter,
                        twc_status_id Id,
                        twc_collections_entriess_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair MaxPosition_KV;
    if (Params.MaxPosition.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxPosition.Value, Buf);
        MaxPosition_KV = twc_KeyValue("max_position", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxPosition_KV);
    }

    twc_key_value_pair MinPosition_KV;
    if (Params.MinPosition.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MinPosition.Value, Buf);
        MinPosition_KV = twc_KeyValue("min_position", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MinPosition_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_COLLECTIONS_ENTRIES, ParamList);
}

extern twc_call_result
twc_Collections_List(twc_state* Twitter,
                     twc_user_id UserId,
                     twc_string ScreenName,
                     twc_collections_list_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(UserId, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV = twc_KeyValueStr("screen_name", ScreenName);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);

    twc_key_value_pair TweetId_KV;
    if (Params.TweetId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.TweetId.Value, Buf);
        TweetId_KV = twc_KeyValue("tweet_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TweetId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_COLLECTIONS_LIST, ParamList);
}

extern twc_call_result
twc_Collections_Show(twc_state* Twitter,
                     twc_status_id Id,
                     twc_collections_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_COLLECTIONS_SHOW, ParamList);
}

extern twc_call_result
twc_DirectMessages(twc_state* Twitter,
                   twc_directmessages_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_DIRECTMESSAGES, ParamList);
}

extern twc_call_result
twc_DirectMessages_Sent(twc_state* Twitter,
                        twc_directmessages_sento_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Page_KV;
    if (Params.Page.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Page.Value, Buf);
        Page_KV = twc_KeyValue("page", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Page_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_DIRECTMESSAGES_SENT, ParamList);
}

extern twc_call_result
twc_DirectMessages_Show(twc_state* Twitter,
                        twc_status_id Id,
                        twc_directmessages_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_DIRECTMESSAGES_SHOW, ParamList);
}

extern twc_call_result
twc_Favorites_List(twc_state* Twitter,
                   twc_favorites_listi_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FAVORITES_LIST, ParamList);
}

extern twc_call_result
twc_Followers_Ids(twc_state* Twitter,
                  twc_followers_ids_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FOLLOWERS_IDS, ParamList);
}

extern twc_call_result
twc_Followers_List(twc_state* Twitter,
                   twc_followers_listl_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
 
    twc_key_value_pair SkipStatus_KV; 
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValue("skip_status", TWC_BOOLSTR(Params.SkipStatus.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
 
    twc_key_value_pair IncludeUserEntities_KV; 
    if (Params.IncludeUserEntities.Exists) {
        IncludeUserEntities_KV = twc_KeyValue("include_user_entities", TWC_BOOLSTR(Params.IncludeUserEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeUserEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FOLLOWERS_LIST, ParamList);
}

extern twc_call_result
twc_Friends_Ids(twc_state* Twitter,
                twc_friends_ids_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDS_IDS, ParamList);
}

extern twc_call_result
twc_Friends_List(twc_state* Twitter,
                 twc_friends_listx_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
 
    twc_key_value_pair SkipStatus_KV; 
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValue("skip_status", TWC_BOOLSTR(Params.SkipStatus.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
 
    twc_key_value_pair IncludeUserEntities_KV; 
    if (Params.IncludeUserEntities.Exists) {
        IncludeUserEntities_KV = twc_KeyValue("include_user_entities", TWC_BOOLSTR(Params.IncludeUserEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeUserEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDS_LIST, ParamList);
}

extern twc_call_result
twc_Friendships_Incoming(twc_state* Twitter,
                         twc_friendships_incoming_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDSHIPS_INCOMING, ParamList);
}

extern twc_call_result
twc_Friendships_Lookup(twc_state* Twitter,
                       twc_friendships_lookup_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDSHIPS_LOOKUP, ParamList);
}

extern twc_call_result
twc_Friendships_NoRetweets_Ids(twc_state* Twitter,
                               twc_friendships_noretweets_ids_params Params)
{
    twc_key_value_list ParamList = NULL;
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDSHIPS_NORETWEETS_IDS, ParamList);
}

extern twc_call_result
twc_Friendships_Outgoing(twc_state* Twitter,
                         twc_friendships_outgoing_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDSHIPS_OUTGOING, ParamList);
}

extern twc_call_result
twc_Friendships_Show(twc_state* Twitter,
                     twc_friendships_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair SourceId_KV;
    if (Params.SourceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SourceId.Value, Buf);
        SourceId_KV = twc_KeyValue("source_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SourceId_KV);
    }

    twc_key_value_pair SourceScreenName_KV;
    if (Params.SourceScreenName.Exists) {
        SourceScreenName_KV = twc_KeyValueStr("source_screen_name", Params.SourceScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SourceScreenName_KV);
    }

    twc_key_value_pair TargetId_KV;
    if (Params.TargetId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.TargetId.Value, Buf);
        TargetId_KV = twc_KeyValue("target_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TargetId_KV);
    }

    twc_key_value_pair TargetScreenName_KV;
    if (Params.TargetScreenName.Exists) {
        TargetScreenName_KV = twc_KeyValueStr("target_screen_name", Params.TargetScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TargetScreenName_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_FRIENDSHIPS_SHOW, ParamList);
}

extern twc_call_result
twc_Geo_Id_PlaceId(twc_state* Twitter,
                   twc_place_id PlaceId,
                   twc_geo_id_placeidlb_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(PlaceId, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_GEO_ID_PLACEID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_GEO_ID_PLACEID, TSTR(IdBuf), FilledURL);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Geo_ReverseGeocode(twc_state* Twitter,
                       twc_string Lat,
                       twc_string Long,
                       twc_geo_reversegeocodew_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Lat_KV = twc_KeyValueStr("lat", Lat);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);

    twc_key_value_pair Long_KV = twc_KeyValueStr("long", Long);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);

    twc_key_value_pair Accuracy_KV;
    if (Params.Accuracy.Exists) {
        Accuracy_KV = twc_KeyValueStr("accuracy", Params.Accuracy.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Accuracy_KV);
    }

    twc_key_value_pair Granularity_KV;
    if (Params.Granularity.Exists) {
        Granularity_KV = twc_KeyValueStr("granularity", Params.Granularity.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Granularity_KV);
    }

    twc_key_value_pair MaxResults_KV;
    if (Params.MaxResults.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.MaxResults.Value, Buf);
        MaxResults_KV = twc_KeyValue("max_results", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxResults_KV);
    }

    twc_key_value_pair Callback_KV;
    if (Params.Callback.Exists) {
        Callback_KV = twc_KeyValueStr("callback", Params.Callback.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Callback_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_GEO_REVERSEGEOCODE, ParamList);
}

extern twc_call_result
twc_Geo_Search(twc_state* Twitter,
               twc_geo_search_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Lat_KV;
    if (Params.Lat.Exists) {
        Lat_KV = twc_KeyValueStr("lat", Params.Lat.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);
    }

    twc_key_value_pair Long_KV;
    if (Params.Long.Exists) {
        Long_KV = twc_KeyValueStr("long", Params.Long.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);
    }

    twc_key_value_pair Query_KV;
    if (Params.Query.Exists) {
        Query_KV = twc_KeyValueStr("query", Params.Query.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Query_KV);
    }

    twc_key_value_pair Ip_KV;
    if (Params.Ip.Exists) {
        Ip_KV = twc_KeyValueStr("ip", Params.Ip.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Ip_KV);
    }

    twc_key_value_pair Granularity_KV;
    if (Params.Granularity.Exists) {
        Granularity_KV = twc_KeyValueStr("granularity", Params.Granularity.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Granularity_KV);
    }

    twc_key_value_pair Accuracy_KV;
    if (Params.Accuracy.Exists) {
        Accuracy_KV = twc_KeyValueStr("accuracy", Params.Accuracy.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Accuracy_KV);
    }

    twc_key_value_pair MaxResults_KV;
    if (Params.MaxResults.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.MaxResults.Value, Buf);
        MaxResults_KV = twc_KeyValue("max_results", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxResults_KV);
    }

    twc_key_value_pair ContainedWithin_KV;
    if (Params.ContainedWithin.Exists) {
        ContainedWithin_KV = twc_KeyValueStr("contained_within", Params.ContainedWithin.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ContainedWithin_KV);
    }

    twc_key_value_pair AttributeStreetAddress_KV;
    if (Params.AttributeStreetAddress.Exists) {
        AttributeStreetAddress_KV = twc_KeyValueStr("attribute:street_address", Params.AttributeStreetAddress.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &AttributeStreetAddress_KV);
    }

    twc_key_value_pair Callback_KV;
    if (Params.Callback.Exists) {
        Callback_KV = twc_KeyValueStr("callback", Params.Callback.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Callback_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_GEO_SEARCH, ParamList);
}

extern twc_call_result
twc_Help_Configuration(twc_state* Twitter,
                       twc_help_configuration_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_HELP_CONFIGURATION, ParamList);
}

extern twc_call_result
twc_Help_Languages(twc_state* Twitter,
                   twc_help_languages_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_HELP_LANGUAGES, ParamList);
}

extern twc_call_result
twc_Help_Privacy(twc_state* Twitter,
                 twc_help_privacy_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_HELP_PRIVACY, ParamList);
}

extern twc_call_result
twc_Help_Tos(twc_state* Twitter,
             twc_help_tos_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_HELP_TOS, ParamList);
}

extern twc_call_result
twc_Lists_List(twc_state* Twitter,
               twc_lists_list_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }
 
    twc_key_value_pair Reverse_KV; 
    if (Params.Reverse.Exists) {
        Reverse_KV = twc_KeyValue("reverse", TWC_BOOLSTR(Params.Reverse.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Reverse_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_LIST, ParamList);
}

extern twc_call_result
twc_Lists_Members(twc_state* Twitter,
                  twc_status_id ListId,
                  twc_string Slug,
                  twc_lists_members_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_MEMBERS, ParamList);
}

extern twc_call_result
twc_Lists_Members_Show(twc_state* Twitter,
                       twc_status_id ListId,
                       twc_string Slug,
                       twc_user_id UserId,
                       twc_string ScreenName,
                       twc_lists_members_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair UserId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(UserId, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV = twc_KeyValueStr("screen_name", ScreenName);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair IncludeEntities_KV;
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValueStr("include_entities", Params.IncludeEntities.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_MEMBERS_SHOW, ParamList);
}

extern twc_call_result
twc_Lists_Memberships(twc_state* Twitter,
                      twc_lists_memberships_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }

    twc_key_value_pair FilterToOwnedLists_KV;
    if (Params.FilterToOwnedLists.Exists) {
        FilterToOwnedLists_KV = twc_KeyValueStr("filter_to_owned_lists", Params.FilterToOwnedLists.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &FilterToOwnedLists_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_MEMBERSHIPS, ParamList);
}

extern twc_call_result
twc_Lists_Ownerships(twc_state* Twitter,
                     twc_lists_ownerships_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_OWNERSHIPS, ParamList);
}

extern twc_call_result
twc_Lists_Show(twc_state* Twitter,
               twc_status_id ListId,
               twc_string Slug,
               twc_lists_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_SHOW, ParamList);
}

extern twc_call_result
twc_Lists_Statuses(twc_state* Twitter,
                   twc_status_id ListId,
                   twc_string Slug,
                   twc_lists_statuses_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair IncludeEntities_KV;
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValueStr("include_entities", Params.IncludeEntities.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair IncludeRts_KV;
    if (Params.IncludeRts.Exists) {
        IncludeRts_KV = twc_KeyValueStr("include_rts", Params.IncludeRts.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeRts_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_STATUSES, ParamList);
}

extern twc_call_result
twc_Lists_Subscribers(twc_state* Twitter,
                      twc_status_id ListId,
                      twc_string Slug,
                      twc_lists_subscribers_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }

    twc_key_value_pair IncludeEntities_KV;
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValueStr("include_entities", Params.IncludeEntities.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_SUBSCRIBERS, ParamList);
}

extern twc_call_result
twc_Lists_Subscribers_Show(twc_state* Twitter,
                           twc_status_id ListId,
                           twc_string Slug,
                           twc_user_id UserId,
                           twc_string ScreenName,
                           twc_lists_subscribers_show_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair UserId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(UserId, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV = twc_KeyValueStr("screen_name", ScreenName);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);

    twc_key_value_pair IncludeEntities_KV;
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValueStr("include_entities", Params.IncludeEntities.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_SUBSCRIBERS_SHOW, ParamList);
}

extern twc_call_result
twc_Lists_Subscriptions(twc_state* Twitter,
                        twc_lists_subscriptions_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_LISTS_SUBSCRIPTIONS, ParamList);
}

extern twc_call_result
twc_Mutes_Users_Ids(twc_state* Twitter,
                    twc_mutes_users_ids_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_MUTES_USERS_IDS, ParamList);
}

extern twc_call_result
twc_Mutes_Users_List(twc_state* Twitter,
                     twc_mutes_users_liste_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
 
    twc_key_value_pair SkipStatus_KV; 
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValue("skip_status", TWC_BOOLSTR(Params.SkipStatus.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_MUTES_USERS_LIST, ParamList);
}

extern twc_call_result
twc_Projects(twc_state* Twitter,
             twc_string Auth,
             twc_projectsm_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Limit_KV;
    if (Params.Limit.Exists) {
        Limit_KV = twc_KeyValueStr("limit", Params.Limit.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Limit_KV);
    }

    twc_key_value_pair Format_KV;
    if (Params.Format.Exists) {
        Format_KV = twc_KeyValueStr("format", Params.Format.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Format_KV);
    }

    twc_key_value_pair Auth_KV = twc_KeyValueStr("auth", Auth);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Auth_KV);
    return twc_MakeCall(Twitter, 0, TWC_URL_PROJECTS, ParamList);
}

extern twc_call_result
twc_SavedSearches_List(twc_state* Twitter,
                       twc_savedsearches_listo_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_SAVEDSEARCHES_LIST, ParamList);
}

extern twc_call_result
twc_SavedSearches_Show_Id(twc_state* Twitter,
                          const char* Id,
                          twc_savedsearches_show_idt_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_SAVEDSEARCHES_SHOW_ID, TSTR(Id));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_SAVEDSEARCHES_SHOW_ID, TSTR(Id), FilledURL);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Search_Tweets(twc_state* Twitter,
                  twc_string Q,
                  twc_search_tweetsx_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Q_KV = twc_KeyValueStr("q", Q);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Q_KV);

    twc_key_value_pair Geocode_KV;
    if (Params.Geocode.Exists) {
        Geocode_KV = twc_KeyValueStr("geocode", Params.Geocode.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Geocode_KV);
    }

    twc_key_value_pair Lang_KV;
    if (Params.Lang.Exists) {
        Lang_KV = twc_KeyValueStr("lang", Params.Lang.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lang_KV);
    }

    twc_key_value_pair Locale_KV;
    if (Params.Locale.Exists) {
        Locale_KV = twc_KeyValueStr("locale", Params.Locale.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Locale_KV);
    }

    twc_key_value_pair ResultType_KV;
    if (Params.ResultType.Exists) {
        ResultType_KV = twc_KeyValueStr("result_type", Params.ResultType.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ResultType_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair Until_KV;
    if (Params.Until.Exists) {
        Until_KV = twc_KeyValueStr("until", Params.Until.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Until_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_SEARCH_TWEETS, ParamList);
}

extern twc_call_result
twc_Statuses_HomeTimeline(twc_state* Twitter,
                          twc_statuses_hometimeline_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair ExcludeReplies_KV; 
    if (Params.ExcludeReplies.Exists) {
        ExcludeReplies_KV = twc_KeyValue("exclude_replies", TWC_BOOLSTR(Params.ExcludeReplies.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ExcludeReplies_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_HOMETIMELINE, ParamList);
}

extern twc_call_result
twc_Statuses_Lookup(twc_state* Twitter,
                    twc_status_id Id,
                    twc_statuses_lookup_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair Map_KV; 
    if (Params.Map.Exists) {
        Map_KV = twc_KeyValue("map", TWC_BOOLSTR(Params.Map.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Map_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_LOOKUP, ParamList);
}

extern twc_call_result
twc_Statuses_MentionsTimeline(twc_state* Twitter,
                              twc_statuses_mentionstimeline_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_MENTIONSTIMELINE, ParamList);
}

extern twc_call_result
twc_Statuses_Oembed(twc_state* Twitter,
                    twc_string Url,
                    twc_statuses_oembed_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Url_KV = twc_KeyValueStr("url", Url);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Url_KV);

    twc_key_value_pair Maxwidth_KV;
    if (Params.Maxwidth.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Maxwidth.Value, Buf);
        Maxwidth_KV = twc_KeyValue("maxwidth", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Maxwidth_KV);
    }
 
    twc_key_value_pair HideMedia_KV; 
    if (Params.HideMedia.Exists) {
        HideMedia_KV = twc_KeyValue("hide_media", TWC_BOOLSTR(Params.HideMedia.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &HideMedia_KV);
    }
 
    twc_key_value_pair HideThread_KV; 
    if (Params.HideThread.Exists) {
        HideThread_KV = twc_KeyValue("hide_thread", TWC_BOOLSTR(Params.HideThread.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &HideThread_KV);
    }
 
    twc_key_value_pair OmitScript_KV; 
    if (Params.OmitScript.Exists) {
        OmitScript_KV = twc_KeyValue("omit_script", TWC_BOOLSTR(Params.OmitScript.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OmitScript_KV);
    }

    twc_key_value_pair Align_KV;
    if (Params.Align.Exists) {
        Align_KV = twc_KeyValueStr("align", Params.Align.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Align_KV);
    }

    twc_key_value_pair Related_KV;
    if (Params.Related.Exists) {
        Related_KV = twc_KeyValueStr("related", Params.Related.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Related_KV);
    }

    twc_key_value_pair Lang_KV;
    if (Params.Lang.Exists) {
        Lang_KV = twc_KeyValueStr("lang", Params.Lang.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lang_KV);
    }

    twc_key_value_pair Theme_KV;
    if (Params.Theme.Exists) {
        Theme_KV = twc_KeyValueStr("theme", Params.Theme.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Theme_KV);
    }

    twc_key_value_pair LinkColor_KV;
    if (Params.LinkColor != NULL) {
        LinkColor_KV = twc_KeyValue("link_color", Params.LinkColor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &LinkColor_KV);
    }

    twc_key_value_pair WidgetType_KV;
    if (Params.WidgetType.Exists) {
        WidgetType_KV = twc_KeyValueStr("widget_type", Params.WidgetType.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &WidgetType_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_OEMBED, ParamList);
}

extern twc_call_result
twc_Statuses_Retweeters_Ids(twc_state* Twitter,
                            twc_status_id Id,
                            twc_statuses_retweeters_idsq_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair Cursor_KV;
    if (Params.Cursor != NULL) {
        Cursor_KV = twc_KeyValue("cursor", Params.Cursor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Cursor_KV);
    }
 
    twc_key_value_pair StringifyIds_KV; 
    if (Params.StringifyIds.Exists) {
        StringifyIds_KV = twc_KeyValue("stringify_ids", TWC_BOOLSTR(Params.StringifyIds.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StringifyIds_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_RETWEETERS_IDS, ParamList);
}

extern twc_call_result
twc_Statuses_Retweets_Id(twc_state* Twitter,
                         twc_status_id Id,
                         twc_statuses_retweets_idy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(Id, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_STATUSES_RETWEETS_ID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_STATUSES_RETWEETS_ID, TSTR(IdBuf), FilledURL);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_RetweetsOfMe(twc_state* Twitter,
                          twc_statuses_retweetsofme_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
 
    twc_key_value_pair IncludeUserEntities_KV; 
    if (Params.IncludeUserEntities.Exists) {
        IncludeUserEntities_KV = twc_KeyValue("include_user_entities", TWC_BOOLSTR(Params.IncludeUserEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeUserEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_RETWEETSOFME, ParamList);
}

extern twc_call_result
twc_Statuses_Show_Id(twc_state* Twitter,
                     twc_status_id Id,
                     twc_statuses_show_id_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(Id, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_STATUSES_SHOW_ID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_STATUSES_SHOW_ID, TSTR(IdBuf), FilledURL);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair IncludeMyRetweet_KV; 
    if (Params.IncludeMyRetweet.Exists) {
        IncludeMyRetweet_KV = twc_KeyValue("include_my_retweet", TWC_BOOLSTR(Params.IncludeMyRetweet.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeMyRetweet_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_UserTimeline(twc_state* Twitter,
                          twc_statuses_usertimeline_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair SinceId_KV;
    if (Params.SinceId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.SinceId.Value, Buf);
        SinceId_KV = twc_KeyValue("since_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SinceId_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }

    twc_key_value_pair MaxId_KV;
    if (Params.MaxId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.MaxId.Value, Buf);
        MaxId_KV = twc_KeyValue("max_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MaxId_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
 
    twc_key_value_pair ExcludeReplies_KV; 
    if (Params.ExcludeReplies.Exists) {
        ExcludeReplies_KV = twc_KeyValue("exclude_replies", TWC_BOOLSTR(Params.ExcludeReplies.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ExcludeReplies_KV);
    }
 
    twc_key_value_pair ContributorDetails_KV; 
    if (Params.ContributorDetails.Exists) {
        ContributorDetails_KV = twc_KeyValue("contributor_details", TWC_BOOLSTR(Params.ContributorDetails.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ContributorDetails_KV);
    }
 
    twc_key_value_pair IncludeRts_KV; 
    if (Params.IncludeRts.Exists) {
        IncludeRts_KV = twc_KeyValue("include_rts", TWC_BOOLSTR(Params.IncludeRts.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeRts_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_STATUSES_USERTIMELINE, ParamList);
}

extern twc_call_result
twc_Trends_Available(twc_state* Twitter,
                     twc_trends_available__params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 0, TWC_URL_TRENDS_AVAILABLE, ParamList);
}

extern twc_call_result
twc_Trends_Closest(twc_state* Twitter,
                   twc_string Lat,
                   twc_string Long,
                   twc_trends_closest_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Lat_KV = twc_KeyValueStr("lat", Lat);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);

    twc_key_value_pair Long_KV = twc_KeyValueStr("long", Long);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);
    return twc_MakeCall(Twitter, 0, TWC_URL_TRENDS_CLOSEST, ParamList);
}

extern twc_call_result
twc_Trends_Place(twc_state* Twitter,
                 twc_status_id Id,
                 twc_trends_placea_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair Exclude_KV;
    if (Params.Exclude.Exists) {
        Exclude_KV = twc_KeyValueStr("exclude", Params.Exclude.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Exclude_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_TRENDS_PLACE, ParamList);
}

extern twc_call_result
twc_Users_Lookup(twc_state* Twitter,
                 twc_users_lookupm_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_USERS_LOOKUP, ParamList);
}

extern twc_call_result
twc_Users_ProfileBanner(twc_state* Twitter,
                        twc_users_profilebannerav_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_USERS_PROFILEBANNER, ParamList);
}

extern twc_call_result
twc_Users_Search(twc_state* Twitter,
                 twc_string Q,
                 twc_users_searchc_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Q_KV = twc_KeyValueStr("q", Q);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Q_KV);

    twc_key_value_pair Page_KV;
    if (Params.Page.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Page.Value, Buf);
        Page_KV = twc_KeyValue("page", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Page_KV);
    }

    twc_key_value_pair Count_KV;
    if (Params.Count.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.Count.Value, Buf);
        Count_KV = twc_KeyValue("count", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Count_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_USERS_SEARCH, ParamList);
}

extern twc_call_result
twc_Users_Show(twc_state* Twitter,
               twc_user_id UserId,
               twc_string ScreenName,
               twc_users_shown_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(UserId, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV = twc_KeyValueStr("screen_name", ScreenName);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_USERS_SHOW, ParamList);
}

extern twc_call_result
twc_Users_Suggestions(twc_state* Twitter,
                      twc_users_suggestionst_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Lang_KV;
    if (Params.Lang.Exists) {
        Lang_KV = twc_KeyValueStr("lang", Params.Lang.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lang_KV);
    }
    return twc_MakeCall(Twitter, 0, TWC_URL_USERS_SUGGESTIONS, ParamList);
}

extern twc_call_result
twc_Users_Suggestions_Slug(twc_state* Twitter,
                           twc_string Slug,
                           twc_users_suggestions_slug_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_USERS_SUGGESTIONS_SLUG, Slug);
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_USERS_SUGGESTIONS_SLUG, Slug, FilledURL);
    }

    twc_key_value_pair Lang_KV;
    if (Params.Lang.Exists) {
        Lang_KV = twc_KeyValueStr("lang", Params.Lang.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lang_KV);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Users_Suggestions_Slug_Members(twc_state* Twitter,
                                   twc_string Slug,
                                   twc_users_suggestions_slug_membersu_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_USERS_SUGGESTIONS_SLUG_MEMBERS, Slug);
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_USERS_SUGGESTIONS_SLUG_MEMBERS, Slug, FilledURL);
    }
    return twc_MakeCall(Twitter, 0, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Account_RemoveProfileBanner(twc_state* Twitter,
                                twc_account_removeprofilebanner_params Params)
{
    twc_key_value_list ParamList = NULL;
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_REMOVEPROFILEBANNER, ParamList);
}

extern twc_call_result
twc_Post_Account_Settings(twc_state* Twitter,
                          twc_post_account_settings_params Params)
{
    twc_key_value_list ParamList = NULL;
 
    twc_key_value_pair SleepTimeEnabled_KV; 
    if (Params.SleepTimeEnabled.Exists) {
        SleepTimeEnabled_KV = twc_KeyValue("sleep_time_enabled", TWC_BOOLSTR(Params.SleepTimeEnabled.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SleepTimeEnabled_KV);
    }

    twc_key_value_pair StartSleepTime_KV;
    if (Params.StartSleepTime.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.StartSleepTime.Value, Buf);
        StartSleepTime_KV = twc_KeyValue("start_sleep_time", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &StartSleepTime_KV);
    }

    twc_key_value_pair EndSleepTime_KV;
    if (Params.EndSleepTime.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.EndSleepTime.Value, Buf);
        EndSleepTime_KV = twc_KeyValue("end_sleep_time", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &EndSleepTime_KV);
    }

    twc_key_value_pair TimeZone_KV;
    if (Params.TimeZone.Exists) {
        TimeZone_KV = twc_KeyValueStr("time_zone", Params.TimeZone.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TimeZone_KV);
    }

    twc_key_value_pair TrendLocationWoeid_KV;
    if (Params.TrendLocationWoeid.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.TrendLocationWoeid.Value, Buf);
        TrendLocationWoeid_KV = twc_KeyValue("trend_location_woeid", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrendLocationWoeid_KV);
    }

    twc_key_value_pair Lang_KV;
    if (Params.Lang.Exists) {
        Lang_KV = twc_KeyValueStr("lang", Params.Lang.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lang_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_SETTINGS, ParamList);
}

extern twc_call_result
twc_Account_UpdateProfile(twc_state* Twitter,
                          twc_account_updateprofilea_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Name_KV;
    if (Params.Name.Exists) {
        Name_KV = twc_KeyValueStr("name", Params.Name.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);
    }

    twc_key_value_pair Url_KV;
    if (Params.Url.Exists) {
        Url_KV = twc_KeyValueStr("url", Params.Url.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Url_KV);
    }

    twc_key_value_pair Location_KV;
    if (Params.Location.Exists) {
        Location_KV = twc_KeyValueStr("location", Params.Location.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Location_KV);
    }

    twc_key_value_pair Description_KV;
    if (Params.Description.Exists) {
        Description_KV = twc_KeyValueStr("description", Params.Description.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Description_KV);
    }

    twc_key_value_pair ProfileLinkColor_KV;
    if (Params.ProfileLinkColor != NULL) {
        ProfileLinkColor_KV = twc_KeyValue("profile_link_color", Params.ProfileLinkColor);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ProfileLinkColor_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_UPDATEPROFILE, ParamList);
}

extern twc_call_result
twc_Account_UpdateProfileBackgroundImage(twc_state* Twitter,
                                         twc_account_updateprofilebackgroundimage_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Image_KV;
    if (Params.Image.Exists) {
        Image_KV = twc_KeyValueStr("image", Params.Image.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Image_KV);
    }

    twc_key_value_pair Tile_KV;
    if (Params.Tile.Exists) {
        Tile_KV = twc_KeyValueStr("tile", Params.Tile.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Tile_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }

    twc_key_value_pair MediaId_KV;
    if (Params.MediaId != NULL) {
        MediaId_KV = twc_KeyValue("media_id", Params.MediaId);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MediaId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_UPDATEPROFILEBACKGROUNDIMAGE, ParamList);
}

extern twc_call_result
twc_Account_UpdateProfileBanner(twc_state* Twitter,
                                twc_string Banner,
                                twc_account_updateprofilebanner_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Banner_KV = twc_KeyValueStr("banner", Banner);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Banner_KV);

    twc_key_value_pair Width_KV;
    if (Params.Width.Exists) {
        Width_KV = twc_KeyValueStr("width", Params.Width.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Width_KV);
    }

    twc_key_value_pair Height_KV;
    if (Params.Height.Exists) {
        Height_KV = twc_KeyValueStr("height", Params.Height.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Height_KV);
    }

    twc_key_value_pair OffsetLeft_KV;
    if (Params.OffsetLeft.Exists) {
        OffsetLeft_KV = twc_KeyValueStr("offset_left", Params.OffsetLeft.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OffsetLeft_KV);
    }

    twc_key_value_pair OffsetTop_KV;
    if (Params.OffsetTop.Exists) {
        OffsetTop_KV = twc_KeyValueStr("offset_top", Params.OffsetTop.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OffsetTop_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_UPDATEPROFILEBANNER, ParamList);
}

extern twc_call_result
twc_Account_UpdateProfileImage(twc_state* Twitter,
                               twc_string Image,
                               twc_account_updateprofileimage_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Image_KV = twc_KeyValueStr("image", Image);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Image_KV);
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_ACCOUNT_UPDATEPROFILEIMAGE, ParamList);
}

extern twc_call_result
twc_Blocks_Create(twc_state* Twitter,
                  twc_blocks_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
 
    twc_key_value_pair SkipStatus_KV; 
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValue("skip_status", TWC_BOOLSTR(Params.SkipStatus.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_BLOCKS_CREATE, ParamList);
}

extern twc_call_result
twc_Blocks_Destroy(twc_state* Twitter,
                   twc_blocks_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }

    twc_key_value_pair SkipStatus_KV;
    if (Params.SkipStatus.Exists) {
        SkipStatus_KV = twc_KeyValueStr("skip_status", Params.SkipStatus.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &SkipStatus_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_BLOCKS_DESTROY, ParamList);
}

extern twc_call_result
twc_Collections_Create(twc_state* Twitter,
                       twc_string Name,
                       twc_collections_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Name_KV = twc_KeyValueStr("name", Name);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);

    twc_key_value_pair Description_KV;
    if (Params.Description.Exists) {
        Description_KV = twc_KeyValueStr("description", Params.Description.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Description_KV);
    }

    twc_key_value_pair Url_KV;
    if (Params.Url.Exists) {
        Url_KV = twc_KeyValueStr("url", Params.Url.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Url_KV);
    }

    twc_key_value_pair TimelineOrder_KV;
    if (Params.TimelineOrder.Exists) {
        TimelineOrder_KV = twc_KeyValueStr("timeline_order", Params.TimelineOrder.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TimelineOrder_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_CREATE, ParamList);
}

extern twc_call_result
twc_Collections_Destroy(twc_state* Twitter,
                        twc_status_id Id,
                        twc_collections_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_DESTROY, ParamList);
}

extern twc_call_result
twc_Collections_Entries_Add(twc_state* Twitter,
                            twc_status_id Id,
                            twc_status_id TweetId,
                            twc_collections_entries_addm_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair TweetId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(TweetId, Buf);
        TweetId_KV = twc_KeyValue("tweet_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TweetId_KV);
    }

    twc_key_value_pair RelativeTo_KV;
    if (Params.RelativeTo.Exists) {
        char Buf[13];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeInt(Params.RelativeTo.Value, Buf);
        RelativeTo_KV = twc_KeyValue("relative_to", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &RelativeTo_KV);
    }
 
    twc_key_value_pair Above_KV; 
    if (Params.Above.Exists) {
        Above_KV = twc_KeyValue("above", TWC_BOOLSTR(Params.Above.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Above_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_ENTRIES_ADD, ParamList);
}

extern twc_call_result
twc_Collections_Entries_Curate(twc_state* Twitter,
                               twc_collections_entries_curatea_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Name_KV;
    if (Params.Name.Exists) {
        Name_KV = twc_KeyValueStr("Name", Params.Name.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_ENTRIES_CURATE, ParamList);
}

extern twc_call_result
twc_Collections_Entries_Move(twc_state* Twitter,
                             twc_status_id Id,
                             twc_status_id TweetId,
                             int RelativeTo,
                             twc_collections_entries_move_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair TweetId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(TweetId, Buf);
        TweetId_KV = twc_KeyValue("tweet_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TweetId_KV);
    }

    twc_key_value_pair RelativeTo_KV;
    char Buf[13];
    memset(Buf, 0, sizeof(Buf));
    twc_SerializeInt(RelativeTo, Buf);
    RelativeTo_KV = twc_KeyValue("relative_to", Buf);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &RelativeTo_KV);
 
    twc_key_value_pair Above_KV; 
    if (Params.Above.Exists) {
        Above_KV = twc_KeyValue("above", TWC_BOOLSTR(Params.Above.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Above_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_ENTRIES_MOVE, ParamList);
}

extern twc_call_result
twc_Collections_Entries_Remove(twc_state* Twitter,
                               twc_status_id Id,
                               twc_status_id TweetId,
                               twc_collections_entries_remove_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair TweetId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(TweetId, Buf);
        TweetId_KV = twc_KeyValue("tweet_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TweetId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_ENTRIES_REMOVE, ParamList);
}

extern twc_call_result
twc_Collections_Update(twc_state* Twitter,
                       twc_status_id Id,
                       twc_collections_update_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }

    twc_key_value_pair Name_KV;
    if (Params.Name.Exists) {
        Name_KV = twc_KeyValueStr("name", Params.Name.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);
    }

    twc_key_value_pair Description_KV;
    if (Params.Description.Exists) {
        Description_KV = twc_KeyValueStr("description", Params.Description.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Description_KV);
    }

    twc_key_value_pair Url_KV;
    if (Params.Url.Exists) {
        Url_KV = twc_KeyValueStr("url", Params.Url.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Url_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_COLLECTIONS_UPDATE, ParamList);
}

extern twc_call_result
twc_DirectMessages_Destroy(twc_state* Twitter,
                           twc_status_id Id,
                           twc_directmessages_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_DIRECTMESSAGES_DESTROY, ParamList);
}

extern twc_call_result
twc_DirectMessages_New(twc_state* Twitter,
                       twc_string Text,
                       twc_directmessages_newj_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair Text_KV = twc_KeyValueStr("text", Text);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Text_KV);
    return twc_MakeCall(Twitter, 1, TWC_URL_DIRECTMESSAGES_NEW, ParamList);
}

extern twc_call_result
twc_Favorites_Create(twc_state* Twitter,
                     twc_status_id Id,
                     twc_favorites_creater_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_FAVORITES_CREATE, ParamList);
}

extern twc_call_result
twc_Favorites_Destroy(twc_state* Twitter,
                      twc_status_id Id,
                      twc_favorites_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Id_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Id, Buf);
        Id_KV = twc_KeyValue("id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Id_KV);
    }
 
    twc_key_value_pair IncludeEntities_KV; 
    if (Params.IncludeEntities.Exists) {
        IncludeEntities_KV = twc_KeyValue("include_entities", TWC_BOOLSTR(Params.IncludeEntities.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &IncludeEntities_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_FAVORITES_DESTROY, ParamList);
}

extern twc_call_result
twc_Friendships_Create(twc_state* Twitter,
                       twc_friendships_createe_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
 
    twc_key_value_pair Follow_KV; 
    if (Params.Follow.Exists) {
        Follow_KV = twc_KeyValue("follow", TWC_BOOLSTR(Params.Follow.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Follow_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_FRIENDSHIPS_CREATE, ParamList);
}

extern twc_call_result
twc_Friendships_Destroy(twc_state* Twitter,
                        twc_friendships_destroym_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_FRIENDSHIPS_DESTROY, ParamList);
}

extern twc_call_result
twc_Friendships_Update(twc_state* Twitter,
                       twc_friendships_updatey_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair Device_KV;
    if (Params.Device.Exists) {
        Device_KV = twc_KeyValueStr("device", Params.Device.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Device_KV);
    }

    twc_key_value_pair Retweets_KV;
    if (Params.Retweets.Exists) {
        Retweets_KV = twc_KeyValueStr("retweets", Params.Retweets.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Retweets_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_FRIENDSHIPS_UPDATE, ParamList);
}

extern twc_call_result
twc_Geo_Place(twc_state* Twitter,
              twc_string Name,
              twc_string ContainedWithin,
              twc_string Token,
              twc_string Lat,
              twc_string Long,
              twc_geo_placez_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Name_KV = twc_KeyValueStr("name", Name);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);

    twc_key_value_pair ContainedWithin_KV = twc_KeyValueStr("contained_within", ContainedWithin);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ContainedWithin_KV);

    twc_key_value_pair Token_KV = twc_KeyValueStr("token", Token);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Token_KV);

    twc_key_value_pair Lat_KV = twc_KeyValueStr("lat", Lat);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);

    twc_key_value_pair Long_KV = twc_KeyValueStr("long", Long);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);

    twc_key_value_pair AttributeStreetAddress_KV;
    if (Params.AttributeStreetAddress.Exists) {
        AttributeStreetAddress_KV = twc_KeyValueStr("attribute:street_address", Params.AttributeStreetAddress.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &AttributeStreetAddress_KV);
    }

    twc_key_value_pair Callback_KV;
    if (Params.Callback.Exists) {
        Callback_KV = twc_KeyValueStr("callback", Params.Callback.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Callback_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_GEO_PLACE, ParamList);
}

extern twc_call_result
twc_Lists_Create(twc_state* Twitter,
                 twc_string Name,
                 twc_lists_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Name_KV = twc_KeyValueStr("name", Name);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);

    twc_key_value_pair Mode_KV;
    if (Params.Mode.Exists) {
        Mode_KV = twc_KeyValueStr("mode", Params.Mode.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Mode_KV);
    }

    twc_key_value_pair Description_KV;
    if (Params.Description.Exists) {
        Description_KV = twc_KeyValueStr("description", Params.Description.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Description_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_CREATE, ParamList);
}

extern twc_call_result
twc_Lists_Destroy(twc_state* Twitter,
                  twc_status_id ListId,
                  twc_string Slug,
                  twc_lists_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_DESTROY, ParamList);
}

extern twc_call_result
twc_Lists_Members_Create(twc_state* Twitter,
                         twc_status_id ListId,
                         twc_string Slug,
                         twc_user_id UserId,
                         twc_string ScreenName,
                         twc_lists_members_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair UserId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(UserId, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV = twc_KeyValueStr("screen_name", ScreenName);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_MEMBERS_CREATE, ParamList);
}

extern twc_call_result
twc_Lists_Members_CreateAll(twc_state* Twitter,
                            twc_status_id ListId,
                            twc_string Slug,
                            twc_lists_members_createall_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_MEMBERS_CREATEALL, ParamList);
}

extern twc_call_result
twc_Lists_Members_Destroy(twc_state* Twitter,
                          twc_lists_members_destroya_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    if (Params.ListId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.ListId.Value, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV;
    if (Params.Slug.Exists) {
        Slug_KV = twc_KeyValueStr("slug", Params.Slug.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_MEMBERS_DESTROY, ParamList);
}

extern twc_call_result
twc_Lists_Members_DestroyAll(twc_state* Twitter,
                             twc_status_id ListId,
                             twc_string Slug,
                             twc_lists_members_destroyall_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_MEMBERS_DESTROYALL, ParamList);
}

extern twc_call_result
twc_Lists_Subscribers_Create(twc_state* Twitter,
                             twc_status_id ListId,
                             twc_string Slug,
                             twc_lists_subscribers_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_SUBSCRIBERS_CREATE, ParamList);
}

extern twc_call_result
twc_Lists_Subscribers_Destroy(twc_state* Twitter,
                              twc_status_id ListId,
                              twc_string Slug,
                              twc_lists_subscribers_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_SUBSCRIBERS_DESTROY, ParamList);
}

extern twc_call_result
twc_Lists_Update(twc_state* Twitter,
                 twc_status_id ListId,
                 twc_string Slug,
                 twc_lists_update_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ListId_KV;
    {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(ListId, Buf);
        ListId_KV = twc_KeyValue("list_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ListId_KV);
    }

    twc_key_value_pair Slug_KV = twc_KeyValueStr("slug", Slug);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Slug_KV);

    twc_key_value_pair Name_KV;
    if (Params.Name.Exists) {
        Name_KV = twc_KeyValueStr("name", Params.Name.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Name_KV);
    }

    twc_key_value_pair Mode_KV;
    if (Params.Mode.Exists) {
        Mode_KV = twc_KeyValueStr("mode", Params.Mode.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Mode_KV);
    }

    twc_key_value_pair Description_KV;
    if (Params.Description.Exists) {
        Description_KV = twc_KeyValueStr("description", Params.Description.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Description_KV);
    }

    twc_key_value_pair OwnerScreenName_KV;
    if (Params.OwnerScreenName.Exists) {
        OwnerScreenName_KV = twc_KeyValueStr("owner_screen_name", Params.OwnerScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerScreenName_KV);
    }

    twc_key_value_pair OwnerId_KV;
    if (Params.OwnerId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.OwnerId.Value, Buf);
        OwnerId_KV = twc_KeyValue("owner_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &OwnerId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_LISTS_UPDATE, ParamList);
}

extern twc_call_result
twc_Mutes_Users_Create(twc_state* Twitter,
                       twc_mutes_users_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_MUTES_USERS_CREATE, ParamList);
}

extern twc_call_result
twc_Mutes_Users_Destroy(twc_state* Twitter,
                        twc_mutes_users_destroy_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_MUTES_USERS_DESTROY, ParamList);
}

extern twc_call_result
twc_SavedSearches_Create(twc_state* Twitter,
                         twc_string Query,
                         twc_savedsearches_create_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Query_KV = twc_KeyValueStr("query", Query);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Query_KV);
    return twc_MakeCall(Twitter, 1, TWC_URL_SAVEDSEARCHES_CREATE, ParamList);
}

extern twc_call_result
twc_SavedSearches_Destroy_Id(twc_state* Twitter,
                             const char* Id,
                             twc_savedsearches_destroy_id__params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_SAVEDSEARCHES_DESTROY_ID, TSTR(Id));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_SAVEDSEARCHES_DESTROY_ID, TSTR(Id), FilledURL);
    }
    return twc_MakeCall(Twitter, 1, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_Destroy_Id(twc_state* Twitter,
                        twc_status_id Id,
                        twc_statuses_destroy_id_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(Id, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_STATUSES_DESTROY_ID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_STATUSES_DESTROY_ID, TSTR(IdBuf), FilledURL);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
    return twc_MakeCall(Twitter, 1, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_Retweet_Id(twc_state* Twitter,
                        twc_status_id Id,
                        twc_statuses_retweet_idr_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(Id, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_STATUSES_RETWEET_ID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_STATUSES_RETWEET_ID, TSTR(IdBuf), FilledURL);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
    return twc_MakeCall(Twitter, 1, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_Unretweet_Id(twc_state* Twitter,
                          twc_status_id Id,
                          twc_statuses_unretweet_idf_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_strbuf FilledURL;
    {
        char IdBuf[TWC_STATUS_ID_BUFSIZE];
        memset(IdBuf, '\0', sizeof(IdBuf));
        twc_SerializeStatusId(Id, IdBuf);

        FilledURL.Size = twc_URLParameterFilledLength(TWC_URL_STATUSES_UNRETWEET_ID, TSTR(IdBuf));
        FilledURL.Ptr = (char*)alloca(FilledURL.Size + 1);
        memset(FilledURL.Ptr, '\0', FilledURL.Size + 1);
        twc_FillURLParameter(TWC_URL_STATUSES_UNRETWEET_ID, TSTR(IdBuf), FilledURL);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }
    return twc_MakeCall(Twitter, 1, FilledURL.Ptr, ParamList);
}

extern twc_call_result
twc_Statuses_Update(twc_state* Twitter,
                    twc_string Status,
                    twc_statuses_updatee_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Status_KV = twc_KeyValueStr("status", Status);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Status_KV);

    twc_key_value_pair InReplyToStatusId_KV;
    if (Params.InReplyToStatusId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.InReplyToStatusId.Value, Buf);
        InReplyToStatusId_KV = twc_KeyValue("in_reply_to_status_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &InReplyToStatusId_KV);
    }
 
    twc_key_value_pair PossiblySensitive_KV; 
    if (Params.PossiblySensitive.Exists) {
        PossiblySensitive_KV = twc_KeyValue("possibly_sensitive", TWC_BOOLSTR(Params.PossiblySensitive.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &PossiblySensitive_KV);
    }

    twc_key_value_pair Lat_KV;
    if (Params.Lat.Exists) {
        Lat_KV = twc_KeyValueStr("lat", Params.Lat.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);
    }

    twc_key_value_pair Long_KV;
    if (Params.Long.Exists) {
        Long_KV = twc_KeyValueStr("long", Params.Long.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);
    }

    twc_key_value_pair PlaceId_KV;
    if (Params.PlaceId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializePlaceId(Params.PlaceId.Value, Buf);
        PlaceId_KV = twc_KeyValue("place_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &PlaceId_KV);
    }
 
    twc_key_value_pair DisplayCoordinates_KV; 
    if (Params.DisplayCoordinates.Exists) {
        DisplayCoordinates_KV = twc_KeyValue("display_coordinates", TWC_BOOLSTR(Params.DisplayCoordinates.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &DisplayCoordinates_KV);
    }
 
    twc_key_value_pair TrimUser_KV; 
    if (Params.TrimUser.Exists) {
        TrimUser_KV = twc_KeyValue("trim_user", TWC_BOOLSTR(Params.TrimUser.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &TrimUser_KV);
    }

    twc_key_value_pair MediaIds_KV;
    if (Params.MediaIds != NULL) {
        MediaIds_KV = twc_KeyValue("media_ids", Params.MediaIds);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &MediaIds_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_STATUSES_UPDATE, ParamList);
}

extern twc_call_result
twc_Statuses_UpdateWithMedia(twc_state* Twitter,
                             twc_string Status,
                             twc_string Media,
                             twc_statuses_updatewithmediam_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair Status_KV = twc_KeyValueStr("status", Status);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Status_KV);

    twc_key_value_pair Media_KV = twc_KeyValueStr("media[]", Media);
    ParamList = twc_KeyValueList_InsertSorted(ParamList, &Media_KV);

    twc_key_value_pair PossiblySensitive_KV;
    if (Params.PossiblySensitive.Exists) {
        PossiblySensitive_KV = twc_KeyValueStr("possibly_sensitive", Params.PossiblySensitive.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &PossiblySensitive_KV);
    }

    twc_key_value_pair InReplyToStatusId_KV;
    if (Params.InReplyToStatusId.Exists) {
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.InReplyToStatusId.Value, Buf);
        InReplyToStatusId_KV = twc_KeyValue("in_reply_to_status_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &InReplyToStatusId_KV);
    }

    twc_key_value_pair Lat_KV;
    if (Params.Lat.Exists) {
        Lat_KV = twc_KeyValueStr("lat", Params.Lat.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Lat_KV);
    }

    twc_key_value_pair Long_KV;
    if (Params.Long.Exists) {
        Long_KV = twc_KeyValueStr("long", Params.Long.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &Long_KV);
    }

    twc_key_value_pair PlaceId_KV;
    if (Params.PlaceId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializePlaceId(Params.PlaceId.Value, Buf);
        PlaceId_KV = twc_KeyValue("place_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &PlaceId_KV);
    }
 
    twc_key_value_pair DisplayCoordinates_KV; 
    if (Params.DisplayCoordinates.Exists) {
        DisplayCoordinates_KV = twc_KeyValue("display_coordinates", TWC_BOOLSTR(Params.DisplayCoordinates.Value));
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &DisplayCoordinates_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_STATUSES_UPDATEWITHMEDIA, ParamList);
}

extern twc_call_result
twc_Users_ReportSpam(twc_state* Twitter,
                     twc_users_reportspamf_params Params)
{
    twc_key_value_list ParamList = NULL;

    twc_key_value_pair ScreenName_KV;
    if (Params.ScreenName.Exists) {
        ScreenName_KV = twc_KeyValueStr("screen_name", Params.ScreenName.Value);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &ScreenName_KV);
    }

    twc_key_value_pair UserId_KV;
    if (Params.UserId.Exists) {
        // User ID is same length as status ID
        char Buf[TWC_STATUS_ID_BUFSIZE];
        memset(Buf, 0, sizeof(Buf));
        twc_SerializeStatusId(Params.UserId.Value, Buf);
        UserId_KV = twc_KeyValue("user_id", Buf);
        ParamList = twc_KeyValueList_InsertSorted(ParamList, &UserId_KV);
    }
    return twc_MakeCall(Twitter, 1, TWC_URL_USERS_REPORTSPAM, ParamList);
}

