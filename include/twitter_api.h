/* account/settings:
Returns settings (including current trend, geo and sleep time information) for the authenticating user. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_get_account_settings_params;

extern twc_call_result
twc_Get_Account_Settings(twc_state* Twitter,
                         twc_get_account_settings_params Params);

/* account/verify_credentials:
Returns an HTTP 200 OK response code and a representation of the requesting user if authentication was successful; returns a 401 status code and an error message if not. Use this method to test if supplied user credentials are valid. */
typedef twc_param_struct {
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user object. */
    bool$ SkipStatus;
    /* When set to true email will be returned in the user objects as a string.
If the user does not have an email address on their account, or if the email
address is not verified, null will be returned. */
    bool$ IncludeEmail;
} twc_account_verifycredentials_params;

extern twc_call_result
twc_Account_VerifyCredentials(twc_state* Twitter,
                              twc_account_verifycredentials_params Params);

/* application/rate_limit_status:
Returns the current rate limits for methods belonging to the specified resource families. Each API resource belongs to a "resource family" which is indicated in its method documentation. The method's resource family can be determined from the first component of the path after the resource version. This method responds with a map of methods belonging to the families specified by the resources parameter, the current remaining uses for each of those resources within the current rate limiting window, and their expiration time in epoch time . It also includes a rate_limit_context field that indicates the current access token or application-only authentication context. You may also issue requests to this method without any parameters to receive a map of all rate limited GET methods. If your application only uses a few of methods, you should explicitly provide a resources parameter with the specified resource families you work with. When using application-only auth, this method's response indicates the application-only auth rate limiting context. Read more about API Rate Limiting and review the limits . */
typedef twc_param_struct {
    /* A comma-separated list of resource families you want to know the current rate
limit disposition for. For best performance, only specify the resource families
pertinent to your application. See API Rate
Limiting for more information. */
    twc_string$ Resources;
} twc_application_ratelimitstatus_params;

extern twc_call_result
twc_Application_RateLimitStatus(twc_state* Twitter,
                                twc_application_ratelimitstatus_params Params);

/* blocks/ids:
Returns an array of numeric user ids the authenticating user is blocking. Important This method is cursored, meaning your app must make multiple requests in order to receive all blocks correctly. See Using cursors to navigate collections for more details on how cursoring works. */
typedef twc_param_struct {
    /* Many programming environments will not consume our ids due to their size.
Provide this option to have ids returned as strings instead. Read more about
Twitter IDs . */
    bool$ StringifyIds;
    /* Causes the list of IDs to be broken into pages of no more than 5000 IDs at a
time. The number of IDs returned is not guaranteed to be 5000 as suspended users
are filtered out after connections are queried. If no cursor is provided, a
value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
} twc_blocks_ids_params;

extern twc_call_result
twc_Blocks_Ids(twc_state* Twitter,
               twc_blocks_ids_params Params);

/* blocks/list:
Returns a collection of user objects that the authenticating user is blocking. Important This method is cursored, meaning your app must make multiple requests in order to receive all blocks correctly. See Using cursors to navigate collections for more details on how cursoring works. */
typedef twc_param_struct {
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
    /* Causes the list of blocked users to be broken into pages of no more than 5000
IDs at a time. The number of IDs returned is not guaranteed to be 5000 as
suspended users are filtered out after connections are queried. If no cursor is
provided, a value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
} twc_blocks_list_params;

extern twc_call_result
twc_Blocks_List(twc_state* Twitter,
                twc_blocks_list_params Params);

/* collections/entries:
Retrieve the identified Collection, presented as a list of the Tweets curated within. The response structure of this method differs significantly from timelines you may be used to working with in the Twitter REST API. To navigate a Collection, use the position object of a response, which includes attributes for max_position , min_position , and was_truncated . was_truncated indicates whether additional Tweets exist in the collection outside of the range of the current request. To retrieve Tweets further back in time, use the value of min_position found in the current response as the
 max_position parameter in your next call to this endpoint. */
typedef twc_param_struct {
    /* Specifies the maximum number of results to include in the response. Specify a
count between 1 and 200. A next_cursor value will be provided in the response
if additional results are available. */
    int$ Count;
    /* Returns results with a position value less than or equal to the specified
position. */
    twc_status_id$ MaxPosition;
    /* Returns results with a position greater than the specified position. */
    twc_status_id$ MinPosition;
} twc_collections_entries_params;

extern twc_call_result
twc_Collections_Entries(twc_state* Twitter,
                        twc_status_id Id,
                        twc_collections_entries_params Params);

/* collections/list:
Find Collections created by a specific user or containing a specific curated Tweet. Results are organized in a cursored collection. */
typedef twc_param_struct {
    /* The identifier of the Tweet for which to return results for. */
    twc_status_id$ TweetId;
    /* Specifies the maximum number of results to include in the response. Specify a
count between 1 and 200. A next_cursor value will be provided in the response
if additional results are available. */
    int$ Count;
    /* A string identifying the segment of the current result set to retrieve. Values
for this parameter are yielded in the cursors node attached to response
objects. Usage of the count parameter affects cursoring. */
    const char* Cursor;
} twc_collections_list_params;

extern twc_call_result
twc_Collections_List(twc_state* Twitter,
                     twc_user_id UserId,
                     twc_string ScreenName,
                     twc_collections_list_params Params);

/* collections/show:
Retrieve information associated with a specific Collection. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_collections_show_params;

extern twc_call_result
twc_Collections_Show(twc_state* Twitter,
                     twc_status_id Id,
                     twc_collections_show_params Params);

/* direct_messages:
Returns the 20 most recent direct messages sent to the authenticating user. Includes detailed information about the sender and recipient user. You can request up to 200 direct messages per call, and only the most recent 200 DMs will be available using this endpoint. Important : This method requires an access token with RWD (read, write & direct message) permissions. Consult The Application Permission Model for more information. */
typedef twc_param_struct {
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* Specifies the number of direct messages to try and retrieve, up to a maximum of
200. The value of count is best thought of as a limit to the number of Tweets
to return because suspended or deleted content is removed after the count has
been applied. */
    int$ Count;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_directmessages_params;

extern twc_call_result
twc_DirectMessages(twc_state* Twitter,
                   twc_directmessages_params Params);

/* direct_messages/sent:
Returns the 20 most recent direct messages sent by the authenticating user. Includes detailed information about the sender and recipient user. You can request up to 200 direct messages per call, up to a maximum of 800 outgoing DMs. Important : This method requires an access token with RWD (read, write & direct message) permissions. Consult The Application Permission Model for more information. */
typedef twc_param_struct {
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* Specifies the number of records to retrieve. Must be less than or equal to 200. */
    int$ Count;
    /* Specifies the page of results to retrieve. */
    int$ Page;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
} twc_directmessages_sent_params;

extern twc_call_result
twc_DirectMessages_Sent(twc_state* Twitter,
                        twc_directmessages_sent_params Params);

/* direct_messages/show:
Returns a single direct message, specified by an id parameter. Like the /1.1/direct_messages.format request, this method will include the user objects of the sender and recipient. Important : This method requires an access token with RWD (read, write & direct message) permissions. Consult The Application Permission Model for more information. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_directmessages_show_params;

extern twc_call_result
twc_DirectMessages_Show(twc_state* Twitter,
                        twc_status_id Id,
                        twc_directmessages_show_params Params);

/* favorites/list:
Returns the 20 most recent Tweets favorited by the authenticating or specified user. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* Specifies the number of records to retrieve. Must be less than or equal to 200;
defaults to 20. The value of count is best thought of as a limit to the number
of tweets to return because suspended or deleted content is removed after the
count has been applied. */
    int$ Count;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* The entities node will be omitted when set to false . */
    bool$ IncludeEntities;
} twc_favorites_list_params;

extern twc_call_result
twc_Favorites_List(twc_state* Twitter,
                   twc_favorites_list_params Params);

/* followers/ids:
Returns a cursored collection of user IDs for every user following the specified user. At this time, results are ordered with the most recent following first u2014 however, this ordering is subject to unannounced change and eventual consistency issues. Results are given in groups of 5,000 user IDs and multiple "pages" of results can be navigated through using the next_cursor value in subsequent requests. See Using cursors to navigate collections for
more information. This method is especially powerful when used in conjunction with GET users / lookup , a method that allows you to convert user IDs into full user objects in bulk. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* Causes the list of connections to be broken into pages of no more than 5000 IDs
at a time. The number of IDs returned is not guaranteed to be 5000 as suspended
users are filtered out after connections are queried. If no cursor is provided,
a value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
    /* Many programming environments will not consume our Tweet ids due to their size.
Provide this option to have ids returned as strings instead. More about
[node:194]. */
    bool$ StringifyIds;
    /* Specifies the number of IDs attempt retrieval of, up to a maximum of 5,000 per
distinct request. The value of count is best thought of as a limit to the
number of results to return. When using the count parameter with this method, it
is wise to use a consistent count value across all requests to the same user's
collection. Usage of this parameter is encouraged in environments where all
5,000 IDs constitutes too large of a response. */
    int$ Count;
} twc_followers_ids_params;

extern twc_call_result
twc_Followers_Ids(twc_state* Twitter,
                  twc_followers_ids_params Params);

/* followers/list:
Returns a cursored collection of user objects for users following the specified user. At this time, results are ordered with the most recent following first u2014 however, this ordering is subject to unannounced change and eventual consistency issues. Results are given in groups of 20 users and multiple "pages" of results can be navigated through using the next_cursor value in subsequent requests. See Using cursors to navigate collections for more
information. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* Causes the results to be broken into pages. If no cursor is provided, a value of
-1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
    /* The number of users to return per page, up to a maximum of 200. Defaults to 20. */
    int$ Count;
    /* When set to either true , t or 1 , statuses will not be included in the
returned user objects. If set to any other value, statuses will be included. */
    bool$ SkipStatus;
    /* The user object entities node will not be included when set to false . */
    bool$ IncludeUserEntities;
} twc_followers_list_params;

extern twc_call_result
twc_Followers_List(twc_state* Twitter,
                   twc_followers_list_params Params);

/* friends/ids:
Returns a cursored collection of user IDs for every user the specified user is following (otherwise known as their "friends"). At this time, results are ordered with the most recent following first u2014 however, this ordering is subject to unannounced change and eventual consistency issues. Results are given in groups of 5,000 user IDs and multiple "pages" of results can be navigated through using the next_cursor value in subsequent requests. See Using cursors to navigate collections for
more information. This method is especially powerful when used in conjunction with GET users / lookup , a method that allows you to convert user IDs into full user objects in bulk. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* Causes the list of connections to be broken into pages of no more than 5000 IDs
at a time. The number of IDs returned is not guaranteed to be 5000 as suspended
users are filtered out after connections are queried. If no cursor is provided,
a value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
    /* Many programming environments will not consume our Tweet ids due to their size.
Provide this option to have ids returned as strings instead. More about Twitter
IDs . */
    bool$ StringifyIds;
    /* Specifies the number of IDs attempt retrieval of, up to a maximum of 5,000 per
distinct request. The value of count is best thought of as a limit to the
number of results to return. When using the count parameter with this method, it
is wise to use a consistent count value across all requests to the same user's
collection. Usage of this parameter is encouraged in environments where all
5,000 IDs constitutes too large of a response. */
    int$ Count;
} twc_friends_ids_params;

extern twc_call_result
twc_Friends_Ids(twc_state* Twitter,
                twc_friends_ids_params Params);

/* friends/list:
Returns a cursored collection of user objects for every user the specified user is following (otherwise known as their "friends"). At this time, results are ordered with the most recent following first u2014 however, this ordering is subject to unannounced change and eventual consistency issues. Results are given in groups of 20 users and multiple "pages" of results can be navigated through using the next_cursor value in subsequent requests. See Using cursors to navigate collections for more
information. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results. */
    twc_string$ ScreenName;
    /* Causes the results to be broken into pages. If no cursor is provided, a value of
-1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See [node:10362, title="Using cursors to
navigate collections"] for more information. */
    const char* Cursor;
    /* The number of users to return per page, up to a maximum of 200. Defaults to 20. */
    int$ Count;
    /* When set to either true, t or 1 statuses will not be included in the
returned user objects. */
    bool$ SkipStatus;
    /* The user object entities node will not be included when set to false. */
    bool$ IncludeUserEntities;
} twc_friends_list_params;

extern twc_call_result
twc_Friends_List(twc_state* Twitter,
                 twc_friends_list_params Params);

/* friendships/incoming:
Returns a collection of numeric IDs for every user who has a pending request to follow the authenticating user. */
typedef twc_param_struct {
    /* Causes the list of connections to be broken into pages of no more than 5000 IDs
at a time. The number of IDs returned is not guaranteed to be 5000 as suspended
users are filtered out after connections are queried. If no cursor is provided,
a value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See [node:10362, title="Using cursors to
navigate collections"] for more information. */
    const char* Cursor;
    /* Many programming environments will not consume our Tweet ids due to their size.
Provide this option to have ids returned as strings instead. More about
[node:194]. */
    bool$ StringifyIds;
} twc_friendships_incoming_params;

extern twc_call_result
twc_Friendships_Incoming(twc_state* Twitter,
                         twc_friendships_incoming_params Params);

/* friendships/lookup:
Returns the relationships of the authenticating user to the comma-separated list of up to 100 screen_names or user_ids provided. Values for connections can be: following , following_requested , followed_by , none , blocking , muting . */
typedef twc_param_struct {
    /* A comma separated list of screen names, up to 100 are allowed in a single
request. */
    twc_string$ ScreenName;
    /* A comma separated list of user IDs, up to 100 are allowed in a single request. */
    twc_user_id$ UserId;
} twc_friendships_lookup_params;

extern twc_call_result
twc_Friendships_Lookup(twc_state* Twitter,
                       twc_friendships_lookup_params Params);

/* friendships/no_retweets/ids:
Returns a collection of user_ids that the currently authenticated user does not want to receive retweets from. Use POST friendships / update to set the "no retweets" status for a given user account on behalf of the current user. */
typedef twc_param_struct {
    /* Many programming environments will not consume our ids due to their size.
Provide this option to have ids returned as strings instead. Read more about
[node:194]. This parameter is especially important to use in Javascript
environments. */
    bool$ StringifyIds;
} twc_friendships_noretweets_ids_params;

extern twc_call_result
twc_Friendships_NoRetweets_Ids(twc_state* Twitter,
                               twc_friendships_noretweets_ids_params Params);

/* friendships/outgoing:
Returns a collection of numeric IDs for every protected user for whom the authenticating user has a pending follow request. */
typedef twc_param_struct {
    /* Causes the list of connections to be broken into pages of no more than 5000 IDs
at a time. The number of IDs returned is not guaranteed to be 5000 as suspended
users are filtered out after connections are queried. If no cursor is provided,
a value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See [node:10362, title="Using cursors to
navigate collections"] for more information. */
    const char* Cursor;
    /* Many programming environments will not consume our Tweet ids due to their size.
Provide this option to have ids returned as strings instead. More about
[node:194]. */
    bool$ StringifyIds;
} twc_friendships_outgoing_params;

extern twc_call_result
twc_Friendships_Outgoing(twc_state* Twitter,
                         twc_friendships_outgoing_params Params);

/* friendships/show:
Returns detailed information about the relationship between two arbitrary users. */
typedef twc_param_struct {
    /* The user_id of the subject user. */
    twc_status_id$ SourceId;
    /* The screen_name of the subject user. */
    twc_string$ SourceScreenName;
    /* The user_id of the target user. */
    twc_status_id$ TargetId;
    /* The screen_name of the target user. */
    twc_string$ TargetScreenName;
} twc_friendships_show_params;

extern twc_call_result
twc_Friendships_Show(twc_state* Twitter,
                     twc_friendships_show_params Params);

/* geo/id/:place_id:
Returns all the information about a known place . */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_geo_id_placeid_params;

extern twc_call_result
twc_Geo_Id_PlaceId(twc_state* Twitter,
                   twc_place_id PlaceId,
                   twc_geo_id_placeid_params Params);

/* geo/reverse_geocode:
Given a latitude and a longitude, searches for up to 20 places that can be used as a place_id when updating a status. This request is an informative call and will deliver generalized results about geography. */
typedef twc_param_struct {
    /* A hint on the "region" in which to search. If a number, then this is a radius in
meters, but it can also take a string that is suffixed with ft to specify feet.
If this is not passed in, then it is assumed to be 0m . If coming from a
device, in practice, this value is whatever accuracy the device has measuring
its location (whether it be coming from a GPS, WiFi triangulation, etc.). */
    twc_string$ Accuracy;
    /* This is the minimal granularity of place types to return and must be one of:
poi , neighborhood , city , admin or country . If no granularity is
provided for the request neighborhood is assumed.
Setting this to city , for example, will find places which have a type of
city , admin or country . */
    twc_string$ Granularity;
    /* A hint as to the number of results to return. This does not guarantee that the
number of results returned will equal max_results, but instead informs how many
"nearby" results to return. Ideally, only pass in the number of places you
intend to display to the user here. */
    int$ MaxResults;
    /* If supplied, the response will use the JSONP format with a callback of the given
name. */
    twc_string$ Callback;
} twc_geo_reversegeocode_params;

extern twc_call_result
twc_Geo_ReverseGeocode(twc_state* Twitter,
                       twc_string Lat,
                       twc_string Long,
                       twc_geo_reversegeocode_params Params);

/* geo/search:
Search for places that can be attached to a statuses/update. Given a latitude and a longitude pair, an IP address, or a name, this request will return a list of all the valid places that can be used as the place_id when updating a status. Conceptually, a query can be made from the user's location, retrieve a list of places, have the user validate the location he or she is at, and then send the ID of this location with a call to POST statuses/update . This is the recommended method to use find places that can be attached to statuses/update. Unlike GET geo/reverse_geocode which provides raw data access, this endpoint can potentially re-order places with regards to the user who is authenticated. This approach is also preferred for interactive place matching with the user. Some parameters in this method are only required based on the existence of other parameters. For instance, "lat" is required if "long" is provided, and vice-versa. Authentication is recommended, but not required with this method. */
typedef twc_param_struct {
    /* The latitude to search around. This parameter will be ignored unless it is
inside the range -90.0 to +90.0 (North is positive) inclusive. It will also be
ignored if there isn't a corresponding long parameter. */
    twc_string$ Lat;
    /* The longitude to search around. The valid ranges for longitude is -180.0 to
+180.0 (East is positive) inclusive. This parameter will be ignored if outside
that range, if it is not a number, if geo_enabled is disabled, or if there
not a corresponding lat parameter. */
    twc_string$ Long;
    /* Free-form text to match against while executing a geo-based query, best suited
for finding nearby locations by name. Remember to URL encode the query. */
    twc_string$ Query;
    /* An IP address. Used when attempting to fix geolocation based off of the user's
IP address. */
    twc_string$ Ip;
    /* This is the minimal granularity of place types to return and must be one of:
poi , neighborhood , city , admin or country . If no granularity is
provided for the request neighborhood is assumed.
Setting this to city , for example, will find places which have a type of
city , admin or country . */
    twc_string$ Granularity;
    /* A hint on the "region" in which to search. If a number, then this is a radius in
meters, but it can also take a string that is suffixed with ft to specify feet.
If this is not passed in, then it is assumed to be 0m . If coming from a
device, in practice, this value is whatever accuracy the device has measuring
its location (whether it be coming from a GPS, WiFi triangulation, etc.). */
    twc_string$ Accuracy;
    /* A hint as to the number of results to return. This does not guarantee that the
number of results returned will equal max_results, but instead informs how many
"nearby" results to return. Ideally, only pass in the number of places you
intend to display to the user here. */
    int$ MaxResults;
    /* This is the place_id which you would like to restrict the search results to.
Setting this value means only places within the given place_id will be found.
Specify a place_id. For example, to scope all results to places within "San
Francisco, CA USA", you would specify a place_id of "5a110d312052166f" */
    twc_string$ ContainedWithin;
    /* This parameter searches for places which have this given street address. There
are other well-known, and application specific attributes available. Custom
attributes are also permitted. Learn more about [node:208, title="Place
Attributes"]. */
    twc_string$ AttributeStreetAddress;
    /* If supplied, the response will use the JSONP format with a callback of the given
name. */
    twc_string$ Callback;
} twc_geo_search_params;

extern twc_call_result
twc_Geo_Search(twc_state* Twitter,
               twc_geo_search_params Params);

/* help/configuration:
Returns the current configuration used by Twitter including twitter.com slugs which are not usernames, maximum photo resolutions, and t.co shortened URL length. It is recommended applications request this endpoint when they are loaded, but no more than once a day. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_help_configuration_params;

extern twc_call_result
twc_Help_Configuration(twc_state* Twitter,
                       twc_help_configuration_params Params);

/* help/languages:
Returns the list of languages supported by Twitter along with the language code supported by Twitter. The language code may be formatted as ISO 639-1 alpha-2 ( en ), ISO 639-3 alpha-3 ( msa ), or ISO 639-1 alpha-2 combined with an ISO 3166-1 alpha-2 localization ( zh-tw ). */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_help_languages_params;

extern twc_call_result
twc_Help_Languages(twc_state* Twitter,
                   twc_help_languages_params Params);

/* help/privacy:
Returns Twitter's Privacy Policy . */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_help_privacy_params;

extern twc_call_result
twc_Help_Privacy(twc_state* Twitter,
                 twc_help_privacy_params Params);

/* help/tos:
Returns the Twitter Terms of Service . Note: these are not the same as the Developer Policy . */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_help_tos_params;

extern twc_call_result
twc_Help_Tos(twc_state* Twitter,
             twc_help_tos_params Params);

/* lists/list:
Returns all lists the authenticating or specified user subscribes to, including their own. The user is specified using the user_id or screen_name parameters. If no user is given, the authenticating user is used. This method used to be GET lists in version 1.0 of the API and has been renamed for consistency with other call. A maximum of 100 results will be returned by this call. Subscribed lists are returned first, followed by owned lists. This means that if a user subscribes to 90 lists and owns 20 lists, this method returns 90 subscriptions and 10 owned lists. The reverse method returns owned lists first, so with reverse=true , 20 owned lists and 80 subscriptions would be returned. If your goal is to obtain
every list a user owns or subscribes to, use GET lists / ownerships and/or GET lists / subscriptions instead. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. Helpful for disambiguating
when a valid user ID is also a valid screen name. Note: : Specifies the ID
of the user to get lists from. Helpful for disambiguating when a valid user ID
is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* Set this to true if you would like owned lists to be returned first. See
description above for information on how this parameter works. */
    bool$ Reverse;
} twc_lists_list_params;

extern twc_call_result
twc_Lists_List(twc_state* Twitter,
               twc_lists_list_params Params);

/* lists/members:
Returns the members of the specified list. Private list members will only be shown if the authenticated user owns the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug. */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug. */
    twc_status_id$ OwnerId;
    /* Specifies the number of results to return per page (see cursor below). The
default is 20, with a maximum of 5,000. */
    int$ Count;
    /* Causes the collection of list members to be broken into "pages" of consistent
sizes (specified by the count parameter). If no cursor is provided, a value of
-1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See Using cursors to navigate
collections for more information. */
    const char* Cursor;
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
    /* When set to either true, t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_lists_members_params;

extern twc_call_result
twc_Lists_Members(twc_state* Twitter,
                  twc_status_id ListId,
                  twc_string Slug,
                  twc_lists_members_params Params);

/* lists/members/show:
Check if the specified user is a member of the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
    /* When set to either true , t or 1 , each tweet will include a node called
"entities". This node offers a variety of metadata about the tweet in a discreet
structure, including: user_mentions, urls, and hashtags. While entities are
opt-in on timelines at present, they will be made a default component of output
in the future. See Tweet Entities for more
details. */
    twc_string$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_lists_members_show_params;

extern twc_call_result
twc_Lists_Members_Show(twc_state* Twitter,
                       twc_status_id ListId,
                       twc_string Slug,
                       twc_user_id UserId,
                       twc_string ScreenName,
                       twc_lists_members_show_params Params);

/* lists/memberships:
Returns the lists the specified user has been added to. If user_id or screen_name are not provided the memberships for the authenticating user are returned. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. Helpful for disambiguating
when a valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The amount of results to return per page. Defaults to 20. No more than 1000
results will ever be returned in a single page. */
    int$ Count;
    /* Breaks the results into pages. Provide a value of -1 to begin paging. Provide
values as returned in the response body's next_cursor and previous_cursor
attributes to page back and forth in the list. It is recommended to always use
cursors when the method supports them. See [node:10362] for more information. */
    const char* Cursor;
    /* When set to true , t or 1 , will return just lists the authenticating user
owns, and the user represented by user_id or screen_name is a member of. */
    twc_string$ FilterToOwnedLists;
} twc_lists_memberships_params;

extern twc_call_result
twc_Lists_Memberships(twc_state* Twitter,
                      twc_lists_memberships_params Params);

/* lists/ownerships:
Returns the lists owned by the specified Twitter user. Private lists will only be shown if the authenticated user is also the owner of the lists. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* The amount of results to return per page. Defaults to 20. No more than 1000
results will ever be returned in a single page. */
    int$ Count;
    /* Breaks the results into pages. Provide a value of -1 to begin paging. Provide
values as returned in the response body's next_cursor and previous_cursor
attributes to page back and forth in the list. It is recommended to always use
cursors when the method supports them. See [node:10362] for more information. */
    const char* Cursor;
} twc_lists_ownerships_params;

extern twc_call_result
twc_Lists_Ownerships(twc_state* Twitter,
                     twc_lists_ownerships_params Params);

/* lists/show:
Returns the specified list. Private lists will only be shown if the authenticated user owns the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_show_params;

extern twc_call_result
twc_Lists_Show(twc_state* Twitter,
               twc_status_id ListId,
               twc_string Slug,
               twc_lists_show_params Params);

/* lists/statuses:
Returns a timeline of tweets authored by members of the specified list. Retweets are included by default. Use the include_rts=false parameter to omit retweets. Embedded Timelines is a great way to embed list timelines on your website. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* Specifies the number of results to retrieve per "page." */
    int$ Count;
    /* Entities are ON by default in API 1.1, each tweet includes a node called
"entities". This node offers a variety of metadata about the tweet in a discreet
structure, including: user_mentions, urls, and hashtags. You can omit entities
from the result by using include_entities=false */
    twc_string$ IncludeEntities;
    /* When set to either true , t or 1 , the list timeline will contain native
retweets (if they exist) in addition to the standard stream of tweets. The
output format of retweeted tweets is identical to the representation you see in
home_timeline. */
    twc_string$ IncludeRts;
} twc_lists_statuses_params;

extern twc_call_result
twc_Lists_Statuses(twc_state* Twitter,
                   twc_status_id ListId,
                   twc_string Slug,
                   twc_lists_statuses_params Params);

/* lists/subscribers:
Returns the subscribers of the specified list. Private list subscribers will only be shown if the authenticated user owns the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
    /* Specifies the number of results to return per page (see cursor below). The
default is 20, with a maximum of 5,000. */
    int$ Count;
    /* Breaks the results into pages. A single page contains 20 lists. Provide a value
of -1 to begin paging. Provide values as returned in the response body's
next_cursor and previous_cursor attributes to page back and forth in the
list. See Using cursors to navigate collections
for more information. */
    const char* Cursor;
    /* When set to either true , t or 1 , each tweet will include a node called
"entities". This node offers a variety of metadata about the tweet in a discreet
structure, including: user_mentions, urls, and hashtags. While entities are
opt-in on timelines at present, they will be made a default component of output
in the future. See Tweet Entities for more
details. */
    twc_string$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_lists_subscribers_params;

extern twc_call_result
twc_Lists_Subscribers(twc_state* Twitter,
                      twc_status_id ListId,
                      twc_string Slug,
                      twc_lists_subscribers_params Params);

/* lists/subscribers/show:
Check if the specified user is a subscriber of the specified list. Returns the user if they are subscriber. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
    /* When set to either true , t or 1 , each tweet will include a node called
"entities". This node offers a variety of metadata about the tweet in a discreet
structure, including: user_mentions, urls, and hashtags. While entities are
opt-in on timelines at present, they will be made a default component of output
in the future. See Tweet Entities for more
details. */
    twc_string$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_lists_subscribers_show_params;

extern twc_call_result
twc_Lists_Subscribers_Show(twc_state* Twitter,
                           twc_status_id ListId,
                           twc_string Slug,
                           twc_user_id UserId,
                           twc_string ScreenName,
                           twc_lists_subscribers_show_params Params);

/* lists/subscriptions:
Obtain a collection of the lists the specified user is subscribed to, 20 lists per page by default. Does not include the user's own lists. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. Helpful for disambiguating
when a valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The amount of results to return per page. Defaults to 20. No more than 1000
results will ever be returned in a single page. */
    int$ Count;
    /* Breaks the results into pages. Provide a value of -1 to begin paging. Provide
values as returned in the response body's next_cursor and previous_cursor
attributes to page back and forth in the list. It is recommended to always use
cursors when the method supports them. See [node:10362] for more information. */
    const char* Cursor;
} twc_lists_subscriptions_params;

extern twc_call_result
twc_Lists_Subscriptions(twc_state* Twitter,
                        twc_lists_subscriptions_params Params);

/* mutes/users/ids:
Returns an array of numeric user ids the authenticating user has muted. */
typedef twc_param_struct {
    /* Causes the list of IDs to be broken into pages of no more than 5000 IDs at a
time. The number of IDs returned is not guaranteed to be 5000 as suspended users
are filtered out. If no cursor is provided, a value of -1 will be assumed, which
is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See [node:10362, title="Using cursors to
navigate collections"] for more information. */
    const char* Cursor;
} twc_mutes_users_ids_params;

extern twc_call_result
twc_Mutes_Users_Ids(twc_state* Twitter,
                    twc_mutes_users_ids_params Params);

/* mutes/users/list:
Returns an array of user objects the authenticating user has muted. */
typedef twc_param_struct {
    /* Causes the list of IDs to be broken into pages of no more than 5000 IDs at a
time. The number of IDs returned is not guaranteed to be 5000 as suspended users
are filtered out after connections are queried. If no cursor is provided, a
value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See [node:10362, title="Using cursors to
navigate collections"] for more information. */
    const char* Cursor;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    bool$ SkipStatus;
} twc_mutes_users_list_params;

extern twc_call_result
twc_Mutes_Users_List(twc_state* Twitter,
                     twc_mutes_users_list_params Params);

/* projects:
This endpoint requires special permission to access and is only available for TV broadcast use cases. To request access please visit curator.twitter.com . Returns a list of existing projects available to the user. */
typedef twc_param_struct {
    /* The number of elements to return. Defaults to 500 if not specified. */
    twc_string$ Limit;
    /* The output format request. This param can be either json or xml . It will
default to json . */
    twc_string$ Format;
} twc_projects_params;

extern twc_call_result
twc_Projects(twc_state* Twitter,
             twc_string Auth,
             twc_projects_params Params);

/* saved_searches/list:
Returns the authenticated user's saved search queries. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_savedsearches_list_params;

extern twc_call_result
twc_SavedSearches_List(twc_state* Twitter,
                       twc_savedsearches_list_params Params);

/* saved_searches/show/:id:
Retrieve the information for the saved search represented by the given id. The authenticating user must be the owner of saved search ID being requested. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_savedsearches_show_id_params;

extern twc_call_result
twc_SavedSearches_Show_Id(twc_state* Twitter,
                          const char* Id,
                          twc_savedsearches_show_id_params Params);

/* search/tweets:
Returns a collection of relevant Tweets matching a specified query. Please note that Twitter's search service and, by extension, the Search API is not meant to be an exhaustive source of Tweets. Not all Tweets will be indexed or made available via the search interface. In API v1.1, the response format of the Search API has been improved to return Tweet objects more similar to the objects you'll find across the REST API and platform. However, perspectival attributes (fields that pertain to the perspective of the authenticating user) are not currently supported on this endpoint. To learn how to use Twitter Search effectively, consult our guide to Using the Twitter Search API . See Working with Timelines to learn best practices for navigating results by since_id and max_id . */
typedef twc_param_struct {
    /* Returns tweets by users located within a given radius of the given
latitude/longitude. The location is preferentially taking from the Geotagging
API, but will fall back to their Twitter profile. The parameter value is
specified by " latitude,longitude,radius ", where radius units must be
specified as either " mi " (miles) or " km " (kilometers). Note that you
cannot use the near operator via the API to geocode arbitrary locations; however
you can use this geocode parameter to search near geocodes directly. A maximum
of 1,000 distinct "sub-regions" will be considered when using the radius
modifier. */
    twc_string$ Geocode;
    /* Restricts tweets to the given language, given by an ISO
639-1 code. Language
detection is best-effort. */
    twc_string$ Lang;
    /* Specify the language of the query you are sending (only ja is currently
effective). This is intended for language-specific consumers and the default
should work in the majority of cases. */
    twc_string$ Locale;
    /* Optional. Specifies what type of search results you would prefer to receive. The
current default is "mixed." Valid values include:
* mixed : Include both popular and real time results in the response.
* recent : return only the most recent results in the response
* popular : return only the most popular results in the response. */
    twc_string$ ResultType;
    /* The number of tweets to return per page, up to a maximum of 100. Defaults to 15.
This was formerly the "rpp" parameter in the old Search API. */
    int$ Count;
    /* Returns tweets created before the given date. Date should be formatted as
YYYY-MM-DD. Keep in mind that the search index has a 7-day limit. In other
words, no tweets will be found for a date older than one week. */
    twc_string$ Until;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
} twc_search_tweets_params;

extern twc_call_result
twc_Search_Tweets(twc_state* Twitter,
                  twc_string Q,
                  twc_search_tweets_params Params);

/* statuses/home_timeline:
Returns a collection of the most recent Tweets and retweets posted by the authenticating user and the users they follow. The home timeline is central to how most users interact with the Twitter service. Up to 800 Tweets are obtainable on the home timeline. It is more volatile for users that follow many users or follow users who Tweet frequently. See Working with Timelines for instructions on traversing timelines efficiently. */
typedef twc_param_struct {
    /* Specifies the number of records to retrieve. Must be less than or equal to 200.
Defaults to 20. The value of count is best thought of as a limit to the number
of tweets to return because suspended or deleted content is removed after the
count has been applied. */
    int$ Count;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* When set to either true , t or 1 , each Tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* This parameter will prevent replies from appearing in the returned timeline.
Using exclude_replies with the count parameter will mean you will receive
up-to count Tweets u2014 this is because the count parameter retrieves that many
Tweets before filtering out retweets and replies. */
    bool$ ExcludeReplies;
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
} twc_statuses_hometimeline_params;

extern twc_call_result
twc_Statuses_HomeTimeline(twc_state* Twitter,
                          twc_statuses_hometimeline_params Params);

/* statuses/lookup:
Returns fully-hydrated Tweet objects for up to 100 Tweets per request, as specified by comma-separated values passed to the id parameter. This method is especially useful to get the details (hydrate) a collection of Tweet IDs. GET statuses / show / :id is used to retrieve a single Tweet object. There are a few things to note when using this method. */
typedef twc_param_struct {
    /* The entities node that may appear within embedded statuses will not be
included when set to false. */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 , each Tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* When using the map parameter, Tweets that do not exist or cannot be viewed by
the current user will still have their key represented but with an explicitly
null value paired with it */
    bool$ Map;
} twc_statuses_lookup_params;

extern twc_call_result
twc_Statuses_Lookup(twc_state* Twitter,
                    twc_status_id Id,
                    twc_statuses_lookup_params Params);

/* statuses/mentions_timeline:
Returns the 20 most recent mentions (Tweets containing a users's @screen_name) for the authenticating user. The timeline returned is the equivalent of the one seen when you view your mentions on twitter.com. This method can only return up to 800 tweets. See Working with Timelines for instructions on traversing timelines. */
typedef twc_param_struct {
    /* Specifies the number of Tweets to try and retrieve, up to a maximum of 200. The
value of count is best thought of as a limit to the number of tweets to return
because suspended or deleted content is removed after the count has been
applied. We include retweets in the count, even if include_rts is not
supplied. It is recommended you always send include_rts=1 when using this API
method. */
    int$ Count;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
} twc_statuses_mentionstimeline_params;

extern twc_call_result
twc_Statuses_MentionsTimeline(twc_state* Twitter,
                              twc_statuses_mentionstimeline_params Params);

/* statuses/oembed:
Returns a single Tweet, specified by either a Tweet web URL or the Tweet ID, in an oEmbed -compatible format. The returned HTML snippet will be automatically recognized as an Embedded Tweet when Twitter's widget JavaScript is included on the page . The oEmbed endpoint allows customization of the final appearance of an Embedded Tweet by setting the corresponding properties in HTML markup to be interpreted by Twitter's JavaScript bundled with the HTML response by default. The format of the returned markup may change over time as Twitter adds new features or adjusts its Tweet representation. The Tweet fallback markup is meant to be cached on your servers for up to the suggested cache lifetime specified in the cache_age . */
typedef twc_param_struct {
    /* The maximum width of a rendered Tweet in whole pixels. This value must be
between 220 and 550 inclusive. A supplied value under or over the allowed
range will be returned as the minimum or maximum supported width respectively;
the reset width value will be reflected in the returned width property. Note
that Twitter does not support the oEmbed maxheight parameter. Tweets are
fundamentally text, and are therefore of unpredictable height that cannot be
scaled like an image or video. Relatedly, the oEmbed response will not provide a
value for height. Implementations that need consistent heights for Tweets
should refer to the hide_thread and hide_media parameters below */
    int$ Maxwidth;
    /* When set to true , t, or 1 links in a Tweet are not expanded to
photo, video, or link previews */
    bool$ HideMedia;
    /* When set to true , t, or 1 a collapsed version of the previous Tweet
in a conversation thread will not be displayed when the requested Tweet is
in reply to another Tweet */
    bool$ HideThread;
    /* When set to true , t, or 1 the <script> responsible for loading
widgets.js will not be returned. Your webpages should include their own
reference to widgets.js for use across all Twitter widgets including
Embedded Tweets */
    bool$ OmitScript;
    /* Specifies whether the embedded Tweet should be floated left, right, or center in
the page relative to the parent element. Valid values are left, right,
center, and none */
    twc_string$ Align;
    /* A comma-separated list of Twitter usernames related to your content. This value
will be forwarded to Tweet action intents if a viewer chooses
to reply, like, or retweet the embedded Tweet */
    twc_string$ Related;
    /* Request returned HTML and a rendered Tweet in the specified Twitter language
supported by embedded Tweets */
    twc_string$ Lang;
    /* When set to dark, the Tweet is displayed with light text over
a dark background */
    twc_string$ Theme;
    /* Adjust the color of Tweet text links with a hexadecimal color value */
    const char* LinkColor;
    /* Set to video to return a Twitter Video embed for the given Tweet */
    twc_string$ WidgetType;
} twc_statuses_oembed_params;

extern twc_call_result
twc_Statuses_Oembed(twc_state* Twitter,
                    twc_string Url,
                    twc_statuses_oembed_params Params);

/* statuses/retweeters/ids:
Returns a collection of up to 100 user IDs belonging to users who have retweeted the Tweet specified by the id parameter. This method offers similar data to GET statuses / retweets / :id . */
typedef twc_param_struct {
    /* Causes the list of IDs to be broken into pages of no more than 100 IDs at a
time. The number of IDs returned is not guaranteed to be 100 as suspended users
are filtered out after connections are queried. If no cursor is provided, a
value of -1 will be assumed, which is the first "page."
The response from the API will include a previous_cursor and next_cursor
to allow paging back and forth. See our cursor
docs for more information.
While this method supports the cursor parameter, the entire result set can be
returned in a single cursored collection. Using the count parameter with this
method will not provide segmented cursors for use with this parameter. */
    const char* Cursor;
    /* Many programming environments will not consume Tweet ids due to their size.
Provide this option to have ids returned as strings instead. */
    bool$ StringifyIds;
} twc_statuses_retweeters_ids_params;

extern twc_call_result
twc_Statuses_Retweeters_Ids(twc_state* Twitter,
                            twc_status_id Id,
                            twc_statuses_retweeters_ids_params Params);

/* statuses/retweets/:id:
Returns a collection of the 100 most recent retweets of the Tweet specified by the id parameter. */
typedef twc_param_struct {
    /* Specifies the number of records to retrieve. Must be less than or equal to 100. */
    int$ Count;
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
} twc_statuses_retweets_id_params;

extern twc_call_result
twc_Statuses_Retweets_Id(twc_state* Twitter,
                         twc_status_id Id,
                         twc_statuses_retweets_id_params Params);

/* statuses/retweets_of_me:
Returns the most recent Tweets authored by the authenticating user that have been retweeted by others. This timeline is a subset of the user's GET statuses / user_timeline . See Working with Timelines for instructions on traversing timelines. */
typedef twc_param_struct {
    /* Specifies the number of records to retrieve. Must be less than or equal to 100.
If omitted, 20 will be assumed. */
    int$ Count;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* The tweet entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* The user entities node will not be included when set to false . */
    bool$ IncludeUserEntities;
} twc_statuses_retweetsofme_params;

extern twc_call_result
twc_Statuses_RetweetsOfMe(twc_state* Twitter,
                          twc_statuses_retweetsofme_params Params);

/* statuses/show/:id:
Returns a single Tweet , specified by the id parameter. The Tweet's author will also be embedded within the Tweet. See GET statuses / lookup for getting Tweets in bulk (up to 100 per call). See also Embedded Timelines , Embedded Tweets , and GET statuses/oembed for tools to render Tweets according to Display
Requirements . About Geo If there is no geotag for a status, then there will be an empty <geo/> or "geo"  :  {} . This can only be populated if the user has used the Geotagging API to send a statuses/update. The JSON response mostly uses conventions laid out in GeoJSON. The coordinates that Twitter renders are reversed from the GeoJSON specification (GeoJSON specifies a longitude then a latitude, whereas Twitter represents it as a latitude then a longitude), eg: "geo":  {  "type":"Point",  "coordinates":[37.78029,  -122.39697]  } */
typedef twc_param_struct {
    /* When set to either true , t or 1 , each Tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* When set to either true , t or 1 , any Tweets returned that have been
retweeted by the authenticating user will include an additional
current_user_retweet node, containing the ID of the source status for the
retweet. */
    bool$ IncludeMyRetweet;
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
} twc_statuses_show_id_params;

extern twc_call_result
twc_Statuses_Show_Id(twc_state* Twitter,
                     twc_status_id Id,
                     twc_statuses_show_id_params Params);

/* statuses/user_timeline:
Returns a collection of the most recent Tweets posted by the user indicated by the screen_name or user_id parameters. User timelines belonging to protected users may only be requested when the authenticated user either "owns" the timeline or is an approved follower of the owner. The timeline returned is the equivalent of the one seen as a user's profile on twitter.com . This method can only return up to 3,200 of a user's most recent Tweets. Native retweets of other statuses by the user is included in this total, regardless of whether include_rts is set to false when requesting this resource. See Working with Timelines for instructions on traversing timelines. See Embedded Timelines , Embedded Tweets , and GET statuses/oembed for tools to render Tweets according to Display Requirements . */
typedef twc_param_struct {
    /* The ID of the user for whom to return results for. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results for. */
    twc_string$ ScreenName;
    /* Returns results with an ID greater than (that is, more recent than) the
specified ID. There are limits to the number of Tweets which can be accessed
through the API. If the limit of Tweets has occured since the since_id, the
since_id will be forced to the oldest ID available. */
    twc_status_id$ SinceId;
    /* Specifies the number of Tweets to try and retrieve, up to a maximum of 200 per
distinct request. The value of count is best thought of as a limit to the
number of Tweets to return because suspended or deleted content is removed after
the count has been applied. We include retweets in the count, even if
include_rts is not supplied. It is recommended you always send
include_rts=1 when using this API method. */
    int$ Count;
    /* Returns results with an ID less than (that is, older than) or equal to the
specified ID. */
    twc_status_id$ MaxId;
    /* When set to either true , t or 1 , each Tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* This parameter will prevent replies from appearing in the returned timeline.
Using exclude_replies with the count parameter will mean you will receive
up-to count tweets u2014 this is because the count parameter retrieves that many
Tweets before filtering out retweets and replies. This parameter is only
supported for JSON and XML responses. */
    bool$ ExcludeReplies;
    /* This parameter enhances the contributors element of the status response to
include the screen_name of the contributor. By default only the user_id of the
contributor is included. */
    bool$ ContributorDetails;
    /* When set to false , the timeline will strip any native retweets (though they
will still count toward both the maximal length of the timeline and the slice
selected by the count parameter). Note: If you're using the trim_user parameter
in conjunction with include_rts, the retweets will still contain a full user
object. */
    bool$ IncludeRts;
} twc_statuses_usertimeline_params;

extern twc_call_result
twc_Statuses_UserTimeline(twc_state* Twitter,
                          twc_statuses_usertimeline_params Params);

/* trends/available:
Returns the locations that Twitter has trending topic information for. The response is an array of "locations" that encode the location's WOEID and some other human-readable information such as a canonical name and country the location belongs in. A WOEID is a Yahoo! Where On Earth ID . */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_trends_available_params;

extern twc_call_result
twc_Trends_Available(twc_state* Twitter,
                     twc_trends_available_params Params);

/* trends/closest:
Returns the locations that Twitter has trending topic information for, closest to a specified location. The response is an array of "locations" that encode the location's WOEID and some other human-readable information such as a canonical name and country the location belongs in. A WOEID is a Yahoo! Where On Earth ID . */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_trends_closest_params;

extern twc_call_result
twc_Trends_Closest(twc_state* Twitter,
                   twc_string Lat,
                   twc_string Long,
                   twc_trends_closest_params Params);

/* trends/place:
Returns the top 10 trending topics for a specific WOEID , if trending information is available for it. The response is an array of "trend" objects that encode the name of the trending topic, the query parameter that can be used to search for the topic on Twitter Search , and the Twitter Search URL. This information is cached for 5 minutes. Requesting more frequently than that will not return any more data, and will count against your rate limit usage. */
typedef twc_param_struct {
    /* Setting this equal to hashtags will remove all hashtags from the trends list. */
    twc_string$ Exclude;
} twc_trends_place_params;

extern twc_call_result
twc_Trends_Place(twc_state* Twitter,
                 twc_status_id Id,
                 twc_trends_place_params Params);

/* users/lookup:
Returns fully-hydrated user objects for up to 100 users per request, as specified by comma-separated values passed to the user_id and/or screen_name parameters. This method is especially useful when used in conjunction with collections of user IDs returned from GET friends / ids and GET followers / ids . GET users / show is used to retrieve a single user object. There are a few things to note when using this method. */
typedef twc_param_struct {
    /* A comma separated list of screen names, up to 100 are allowed in a single
request. You are strongly encouraged to use a POST for larger (up to 100 screen
names) requests. */
    twc_string$ ScreenName;
    /* A comma separated list of user IDs, up to 100 are allowed in a single request.
You are strongly encouraged to use a POST for larger requests. */
    twc_user_id$ UserId;
    /* The entities node that may appear within embedded statuses will not be
included when set to false. */
    bool$ IncludeEntities;
} twc_users_lookup_params;

extern twc_call_result
twc_Users_Lookup(twc_state* Twitter,
                 twc_users_lookup_params Params);

/* users/profile_banner:
Returns a map of the available size variations of the specified user's profile banner. If the user has not uploaded a profile banner, a HTTP 404 will be served instead. This method can be used instead of string manipulation on the profile_banner_url returned in user objects as described in Profile Images and Banners . The profile banner data available at each size variant's URL is in PNG format. */
typedef twc_param_struct {
    /* The ID of the user for whom to return results. Helpful for disambiguating when a
valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to return results. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
} twc_users_profilebanner_params;

extern twc_call_result
twc_Users_ProfileBanner(twc_state* Twitter,
                        twc_users_profilebanner_params Params);

/* users/search:
Provides a simple, relevance-based search interface to public user accounts on Twitter. Try querying by topical interest, full name, company name, location, or other criteria. Exact match searches are not supported. Only the first 1,000 matching results are available. */
typedef twc_param_struct {
    /* Specifies the page of results to retrieve. */
    int$ Page;
    /* The number of potential user results to retrieve per page. This value has a
maximum of 20. */
    int$ Count;
    /* The entities node will not be included in embedded Tweet objects when set to
false . */
    bool$ IncludeEntities;
} twc_users_search_params;

extern twc_call_result
twc_Users_Search(twc_state* Twitter,
                 twc_string Q,
                 twc_users_search_params Params);

/* users/show:
Returns a variety of information about the user specified by the required user_id or screen_name parameter. The author's most recent Tweet will be returned inline when possible. GET users / lookup is used to retrieve a bulk collection of user objects. You must be following a protected user to be able to see their most recent Tweet. If you don't follow a protected user, the user's Tweet will be removed. A Tweet will not always be returned in the current_status field. */
typedef twc_param_struct {
    /* The entities node will not be included when set to false. */
    bool$ IncludeEntities;
} twc_users_show_params;

extern twc_call_result
twc_Users_Show(twc_state* Twitter,
               twc_user_id UserId,
               twc_string ScreenName,
               twc_users_show_params Params);

/* users/suggestions:
Access to Twitter's suggested user list. This returns the list of suggested user categories. The category can be used in GET users / suggestions / :slug to get the users in that category. */
typedef twc_param_struct {
    /* Restricts the suggested categories to the requested language. The language must
be specified by the appropriate two letter ISO 639-1 representation. Currently
supported languages are provided by the GET help /
languages API request. Unsupported
language codes will receive English (en) results. If you use lang in this
request, ensure you also include it when requesting the GET users / suggestions
/ :slug list. */
    twc_string$ Lang;
} twc_users_suggestions_params;

extern twc_call_result
twc_Users_Suggestions(twc_state* Twitter,
                      twc_users_suggestions_params Params);

/* users/suggestions/:slug:
Access the users in a given category of the Twitter suggested user list. It is recommended that applications cache this data for no more than one hour. */
typedef twc_param_struct {
    /* Restricts the suggested categories to the requested language. The language must
be specified by the appropriate two letter ISO 639-1 representation. Currently
supported languages are provided by the GET help /
languages API request. Unsupported
language codes will receive English (en) results. If you use lang in this
request, ensure you also include it when requesting the GET users / suggestions
/ :slug list. */
    twc_string$ Lang;
} twc_users_suggestions_slug_params;

extern twc_call_result
twc_Users_Suggestions_Slug(twc_state* Twitter,
                           twc_string Slug,
                           twc_users_suggestions_slug_params Params);

/* users/suggestions/:slug/members:
Access the users in a given category of the Twitter suggested user list and return their most recent status if they are not a protected user. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_users_suggestions_slug_members_params;

extern twc_call_result
twc_Users_Suggestions_Slug_Members(twc_state* Twitter,
                                   twc_string Slug,
                                   twc_users_suggestions_slug_members_params Params);

/* account/remove_profile_banner:
Removes the uploaded profile banner for the authenticating user. Returns HTTP 200 upon success. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_account_removeprofilebanner_params;

extern twc_call_result
twc_Account_RemoveProfileBanner(twc_state* Twitter,
                                twc_account_removeprofilebanner_params Params);

/* account/settings:
Updates the authenticating user's settings. */
typedef twc_param_struct {
    /* When set to true , t or 1 , will enable sleep time for the user. Sleep
time is the time when push or SMS notifications should not be sent to the user. */
    bool$ SleepTimeEnabled;
    /* The hour that sleep time should begin if it is enabled. The value for this
parameter should be provided in
ISO8601 format (i.e. 00-23). The
time is considered to be in the same timezone as the user's time_zone
setting. */
    int$ StartSleepTime;
    /* The hour that sleep time should end if it is enabled. The value for this
parameter should be provided in
ISO8601 format (i.e. 00-23). The
time is considered to be in the same timezone as the user's time_zone
setting. */
    int$ EndSleepTime;
    /* The timezone dates and times should be displayed in for the user. The timezone
must be one of the Rails
TimeZone
names. */
    twc_string$ TimeZone;
    /* The Yahoo! Where On Earth ID to use as the user's default trend location. Global
information is available by using 1 as the WOEID. The woeid must be one of the
locations returned by [node:59]. */
    int$ TrendLocationWoeid;
    /* The language which Twitter should render in for this user. The language must be
specified by the appropriate two letter ISO 639-1 representation. Currently
supported languages are provided by this
endpoint . */
    twc_string$ Lang;
} twc_post_account_settings_params;

extern twc_call_result
twc_Post_Account_Settings(twc_state* Twitter,
                          twc_post_account_settings_params Params);

/* account/update_profile:
Sets some values that users are able to set under the "Account" tab of their settings page. Only the parameters specified will be updated. */
typedef twc_param_struct {
    /* Full name associated with the profile. Maximum of 20 characters. */
    twc_string$ Name;
    /* URL associated with the profile. Will be prepended with "http://" if not
present. Maximum of 100 characters. */
    twc_string$ Url;
    /* The city or country describing where the user of the account is located. The
contents are not normalized or geocoded in any way. Maximum of 30 characters. */
    twc_string$ Location;
    /* A description of the user owning the account. Maximum of 160 characters. */
    twc_string$ Description;
    /* Sets a hex value that controls the color scheme of links used on the
authenticating user's profile page on twitter.com. This must be a valid
hexadecimal value, and may be either three or six characters (ex: F00 or
FF0000). This parameter replaces the deprecated (and separate)
update_profile_colors API method. */
    const char* ProfileLinkColor;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_account_updateprofile_params;

extern twc_call_result
twc_Account_UpdateProfile(twc_state* Twitter,
                          twc_account_updateprofile_params Params);

/* account/update_profile_background_image:
Updates the authenticating user's profile background image. This method can also be used to enable or disable the profile background image. Although each parameter is marked as optional, at least one of image or media_id must be provided when making this request. */
typedef twc_param_struct {
    /* The background image for the profile, base64-encoded. Must be a valid GIF, JPG,
or PNG image of less than 800 kilobytes in size. Images with width larger than
2048 pixels will be forcibly scaled down. The image must be provided as raw
multipart data, not a URL. */
    twc_string$ Image;
    /* Whether or not to tile the background image. If set to true , t or 1 the
background image will be displayed tiled. The image will not be tiled otherwise. */
    twc_string$ Tile;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
    /* Specify the media to use as the background image. More information on upload of
media is available here . */
    const char* MediaId;
} twc_account_updateprofilebackgroundimage_params;

extern twc_call_result
twc_Account_UpdateProfileBackgroundImage(twc_state* Twitter,
                                         twc_account_updateprofilebackgroundimage_params Params);

/* account/update_profile_banner:
Uploads a profile banner on behalf of the authenticating user. More information about sizing variations can be found in User Profile Images and Banners and GET users / profile_banner . Profile banner images are processed asynchronously. The profile_banner_url and its variant sizes will not necessary be available directly after upload. */
typedef twc_param_struct {
    /* The width of the preferred section of the image being uploaded in pixels. Use
with height , offset_left , and offset_top to select the desired region
of the image to use. */
    twc_string$ Width;
    /* The height of the preferred section of the image being uploaded in pixels. Use
with width , offset_left , and offset_top to select the desired region
of the image to use. */
    twc_string$ Height;
    /* The number of pixels by which to offset the uploaded image from the left. Use
with height , width , and offset_top to select the desired region of the
image to use. */
    twc_string$ OffsetLeft;
    /* The number of pixels by which to offset the uploaded image from the top. Use
with height , width , and offset_left to select the desired region of the
image to use. */
    twc_string$ OffsetTop;
} twc_account_updateprofilebanner_params;

extern twc_call_result
twc_Account_UpdateProfileBanner(twc_state* Twitter,
                                twc_string Banner,
                                twc_account_updateprofilebanner_params Params);

/* account/update_profile_image:
Updates the authenticating user's profile image. Note that this method expects raw multipart data, not a URL to an image. This method asynchronously processes the uploaded file before updating the user's profile image URL. You can either update your local cache the next time you request the user's information, or, at least 5 seconds after uploading the image, ask for the updated URL using GET users / show . */
typedef twc_param_struct {
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_account_updateprofileimage_params;

extern twc_call_result
twc_Account_UpdateProfileImage(twc_state* Twitter,
                               twc_string Image,
                               twc_account_updateprofileimage_params Params);

/* blocks/create:
Blocks the specified user from following the authenticating user. In addition the blocked user will not show in the authenticating users mentions or timeline (unless retweeted by another user). If a follow or friend relationship exists it is destroyed. The URL pattern /version/block/create/:screen_name_or_user_id.format is still accepted but not recommended. As a sequence of numbers is a valid screen name we recommend using the screen_name or user_id parameter instead. */
typedef twc_param_struct {
    /* The screen name of the potentially blocked user. Helpful for disambiguating when
a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The ID of the potentially blocked user. Helpful for disambiguating when a valid
user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    bool$ SkipStatus;
} twc_blocks_create_params;

extern twc_call_result
twc_Blocks_Create(twc_state* Twitter,
                  twc_blocks_create_params Params);

/* blocks/destroy:
Un-blocks the user specified in the ID parameter for the authenticating user. Returns the un-blocked user in the requested format when successful. If relationships existed before the block was instated, they will not be restored. */
typedef twc_param_struct {
    /* The screen name of the potentially blocked user. Helpful for disambiguating when
a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The ID of the potentially blocked user. Helpful for disambiguating when a valid
user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
    /* When set to either true , t or 1 statuses will not be included in the
returned user objects. */
    twc_string$ SkipStatus;
} twc_blocks_destroy_params;

extern twc_call_result
twc_Blocks_Destroy(twc_state* Twitter,
                   twc_blocks_destroy_params Params);

/* collections/create:
Create a Collection owned by the currently authenticated user. The API endpoint may refuse to complete the request if the authenticated user has exceeded the total number of allowed collections for his or her account. */
typedef twc_param_struct {
    /* A brief description of this collection in 160 characters or fewer. */
    twc_string$ Description;
    /* A fully-qualified URL to associate with this collection. */
    twc_string$ Url;
    /* Order Tweets chronologically or in the order they are added to a Collection.
curation_reverse_chron - order added (default) tweet_chron - oldest first
tweet_reverse_chron - most recent first */
    twc_string$ TimelineOrder;
} twc_collections_create_params;

extern twc_call_result
twc_Collections_Create(twc_state* Twitter,
                       twc_string Name,
                       twc_collections_create_params Params);

/* collections/destroy:
Permanently delete a Collection owned by the currently authenticated user. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_collections_destroy_params;

extern twc_call_result
twc_Collections_Destroy(twc_state* Twitter,
                        twc_status_id Id,
                        twc_collections_destroy_params Params);

/* collections/entries/add:
Add a specified Tweet to a Collection. A collection will store up to a few thousand Tweets. Adding a Tweet to a collection beyond its allowed capacity will remove the oldest Tweet in the collection based on the time it was added to the collection. Use POST collections / entries / curate to add Tweets to a Collection in bulk. */
typedef twc_param_struct {
    /* The identifier of the Tweet used for relative positioning in a
curation_reverse_chron ordered collection. */
    int$ RelativeTo;
    /* Set to false to insert the specified tweet_id below the relative_to
Tweet in the collection. Default: true */
    bool$ Above;
} twc_collections_entries_add_params;

extern twc_call_result
twc_Collections_Entries_Add(twc_state* Twitter,
                            twc_status_id Id,
                            twc_status_id TweetId,
                            twc_collections_entries_add_params Params);

/* collections/entries/curate:
Curate a Collection by adding or removing Tweets in bulk. Updates must be limited to 100 cumulative additions or removals per request. Use POST collections / entries / add and POST collections / entries / remove to add or remove a single Tweet. */
typedef twc_param_struct {
    /* Description */
    twc_string$ Name;
} twc_collections_entries_curate_params;

extern twc_call_result
twc_Collections_Entries_Curate(twc_state* Twitter,
                               twc_collections_entries_curate_params Params);

/* collections/entries/move:
Move a specified Tweet to a new position in a curation_reverse_chron ordered collection. */
typedef twc_param_struct {
    /* Set to false to insert the specified tweet_id below the relative_to
Tweet in the collection. Default: true */
    bool$ Above;
} twc_collections_entries_move_params;

extern twc_call_result
twc_Collections_Entries_Move(twc_state* Twitter,
                             twc_status_id Id,
                             twc_status_id TweetId,
                             int RelativeTo,
                             twc_collections_entries_move_params Params);

/* collections/entries/remove:
Remove the specified Tweet from a Collection. Use POST collections / entries / curate to remove Tweets from a Collection in bulk. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_collections_entries_remove_params;

extern twc_call_result
twc_Collections_Entries_Remove(twc_state* Twitter,
                               twc_status_id Id,
                               twc_status_id TweetId,
                               twc_collections_entries_remove_params Params);

/* collections/update:
Update information concerning a Collection owned by the currently authenticated user. Partial updates are not currently supported: please provide name , description , and url whenever using this method. Omitted description or url parameters will be treated as if an empty string was passed, overwriting any previously stored value for the Collection. */
typedef twc_param_struct {
    /* The title of the Collection being created, in 25 characters or fewer. */
    twc_string$ Name;
    /* A brief description of this Collection in 160 characters or fewer. */
    twc_string$ Description;
    /* A fully-qualified URL to associate with this Collection. */
    twc_string$ Url;
} twc_collections_update_params;

extern twc_call_result
twc_Collections_Update(twc_state* Twitter,
                       twc_status_id Id,
                       twc_collections_update_params Params);

/* direct_messages/destroy:
Destroys the direct message specified in the required ID parameter. The authenticating user must be the recipient of the specified direct message. Important : This method requires an access token with RWD (read, write & direct message) permissions. Consult The Application Permission Model for more information. */
typedef twc_param_struct {
    /* The entities node will not be included when set to false . */
    bool$ IncludeEntities;
} twc_directmessages_destroy_params;

extern twc_call_result
twc_DirectMessages_Destroy(twc_state* Twitter,
                           twc_status_id Id,
                           twc_directmessages_destroy_params Params);

/* direct_messages/new:
Sends a new direct message to the specified user from the authenticating user. Requires both the user and text parameters and must be a POST. Returns the sent message in the requested format if successful. */
typedef twc_param_struct {
    /* The ID of the user who should receive the direct message. Helpful for
disambiguating when a valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user who should receive the direct message. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
} twc_directmessages_new_params;

extern twc_call_result
twc_DirectMessages_New(twc_state* Twitter,
                       twc_string Text,
                       twc_directmessages_new_params Params);

/* favorites/create:
Favorites the status specified in the ID parameter as the authenticating user. Returns the favorite status when successful. This process invoked by this method is asynchronous. The immediately returned status may not indicate the resultant favorited status of the tweet. A 200 OK response from this method will indicate whether the intended action was successful or not. */
typedef twc_param_struct {
    /* The entities node will be omitted when set to false . */
    bool$ IncludeEntities;
} twc_favorites_create_params;

extern twc_call_result
twc_Favorites_Create(twc_state* Twitter,
                     twc_status_id Id,
                     twc_favorites_create_params Params);

/* favorites/destroy:
Un-favorites the status specified in the ID parameter as the authenticating user. Returns the un-favorited status in the requested format when successful. This process invoked by this method is asynchronous. The immediately returned status may not indicate the resultant favorited status of the tweet. A 200 OK response from this method will indicate whether the intended action was successful or not. */
typedef twc_param_struct {
    /* The entities node will be omitted when set to false . */
    bool$ IncludeEntities;
} twc_favorites_destroy_params;

extern twc_call_result
twc_Favorites_Destroy(twc_state* Twitter,
                      twc_status_id Id,
                      twc_favorites_destroy_params Params);

/* friendships/create:
Allows the authenticating users to follow the user specified in the ID parameter. Returns the befriended user in the requested format when successful. Returns a string describing the failure condition when unsuccessful. If you are already friends with the user a HTTP 403 may be returned, though for performance reasons you may get a 200 OK message even if the friendship already exists. Actions taken in this method are asynchronous and changes will be eventually consistent. */
typedef twc_param_struct {
    /* The screen name of the user for whom to befriend. */
    twc_string$ ScreenName;
    /* The ID of the user for whom to befriend. */
    twc_user_id$ UserId;
    /* Enable notifications for the target user. */
    bool$ Follow;
} twc_friendships_create_params;

extern twc_call_result
twc_Friendships_Create(twc_state* Twitter,
                       twc_friendships_create_params Params);

/* friendships/destroy:
Allows the authenticating user to unfollow the user specified in the ID parameter. Returns the unfollowed user in the requested format when successful. Returns a string describing the failure condition when unsuccessful. Actions taken in this method are asynchronous and changes will be eventually consistent. */
typedef twc_param_struct {
    /* The screen name of the user for whom to unfollow. */
    twc_string$ ScreenName;
    /* The ID of the user for whom to unfollow. */
    twc_user_id$ UserId;
} twc_friendships_destroy_params;

extern twc_call_result
twc_Friendships_Destroy(twc_state* Twitter,
                        twc_friendships_destroy_params Params);

/* friendships/update:
Allows one to enable or disable retweets and device notifications from the specified user. */
typedef twc_param_struct {
    /* The screen name of the user for whom to befriend. */
    twc_string$ ScreenName;
    /* The ID of the user for whom to befriend. */
    twc_user_id$ UserId;
    /* Enable/disable device notifications from the target user. */
    twc_string$ Device;
    /* Enable/disable retweets from the target user. */
    twc_string$ Retweets;
} twc_friendships_update_params;

extern twc_call_result
twc_Friendships_Update(twc_state* Twitter,
                       twc_friendships_update_params Params);

/* geo/place:
As of December 2nd, 2013, this endpoint is deprecated and retired and no longer functions. Place creation was used infrequently by third party applications and is generally no longer supported on Twitter. Requests will return with status 410 (Gone) with error code 251 . Created a new place object at the given latitude and longitude. Before creating a place you needed to query GET geo/similar_places with the latitude, longitude and name of the place you wished to create. The query will return an array of places which are similar to the one you wish to create, and a token . If the place you wished to create wasn't in the returned array you could use the token with this method to
create a new one. Learn more about Finding Tweets about Places . */
typedef twc_param_struct {
    /* This parameter searches for places which have this given street address. There
are other well-known, and application specific attributes available. Custom
attributes are also permitted. Learn more here . */
    twc_string$ AttributeStreetAddress;
    /* If supplied, the response will use the JSONP format with a callback of the given
name. */
    twc_string$ Callback;
} twc_geo_place_params;

extern twc_call_result
twc_Geo_Place(twc_state* Twitter,
              twc_string Name,
              twc_string ContainedWithin,
              twc_string Token,
              twc_string Lat,
              twc_string Long,
              twc_geo_place_params Params);

/* lists/create:
Creates a new list for the authenticated user. Note that you can create up to 1000 lists per account. */
typedef twc_param_struct {
    /* Whether your list is public or private. Values can be public or private . If
no mode is specified the list will be public. */
    twc_string$ Mode;
    /* The description to give the list. */
    twc_string$ Description;
} twc_lists_create_params;

extern twc_call_result
twc_Lists_Create(twc_state* Twitter,
                 twc_string Name,
                 twc_lists_create_params Params);

/* lists/destroy:
Deletes the specified list. The authenticated user must own the list to be able to destroy it. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_destroy_params;

extern twc_call_result
twc_Lists_Destroy(twc_state* Twitter,
                  twc_status_id ListId,
                  twc_string Slug,
                  twc_lists_destroy_params Params);

/* lists/members/create:
Add a member to a list. The authenticated user must own the list to be able to add members to it. Note that lists cannot have more than 5,000 members. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_members_create_params;

extern twc_call_result
twc_Lists_Members_Create(twc_state* Twitter,
                         twc_status_id ListId,
                         twc_string Slug,
                         twc_user_id UserId,
                         twc_string ScreenName,
                         twc_lists_members_create_params Params);

/* lists/members/create_all:
Adds multiple members to a list, by specifying a comma-separated list of member ids or screen names. The authenticated user must own the list to be able to add members to it. Note that lists can't have more than 5,000 members, and you are limited to adding up to 100 members to a list at a time with this method. Please note that there can be issues with lists that rapidly remove and add memberships. Take care when using these methods such that you are not too rapidly switching between removals and adds on the same list. */
typedef twc_param_struct {
    /* A comma separated list of user IDs, up to 100 are allowed in a single request. */
    twc_user_id$ UserId;
    /* A comma separated list of screen names, up to 100 are allowed in a single
request. */
    twc_string$ ScreenName;
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_members_createall_params;

extern twc_call_result
twc_Lists_Members_CreateAll(twc_state* Twitter,
                            twc_status_id ListId,
                            twc_string Slug,
                            twc_lists_members_createall_params Params);

/* lists/members/destroy:
Removes the specified member from the list. The authenticated user must be the list's owner to remove members from the list. */
typedef twc_param_struct {
    /* The numerical id of the list. */
    twc_status_id$ ListId;
    /* You can identify a list by its slug instead of its numerical id. If you decide
to do so, note that you'll also have to specify the list owner using the
owner_id or owner_screen_name parameters. */
    twc_string$ Slug;
    /* The ID of the user to remove from the list. Helpful for disambiguating when a
valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
    /* The screen name of the user for whom to remove from the list. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_members_destroy_params;

extern twc_call_result
twc_Lists_Members_Destroy(twc_state* Twitter,
                          twc_lists_members_destroy_params Params);

/* lists/members/destroy_all:
Removes multiple members from a list, by specifying a comma-separated list of member ids or screen names. The authenticated user must own the list to be able to remove members from it. Note that lists can't have more than 500 members, and you are limited to removing up to 100 members to a list at a time with this method. Please note that there can be issues with lists that rapidly remove and add memberships. Take care when using these methods such that you are not too rapidly switching between removals and adds on the same list. */
typedef twc_param_struct {
    /* A comma separated list of user IDs, up to 100 are allowed in a single request. */
    twc_user_id$ UserId;
    /* A comma separated list of screen names, up to 100 are allowed in a single
request. */
    twc_string$ ScreenName;
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_members_destroyall_params;

extern twc_call_result
twc_Lists_Members_DestroyAll(twc_state* Twitter,
                             twc_status_id ListId,
                             twc_string Slug,
                             twc_lists_members_destroyall_params Params);

/* lists/subscribers/create:
Subscribes the authenticated user to the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_subscribers_create_params;

extern twc_call_result
twc_Lists_Subscribers_Create(twc_state* Twitter,
                             twc_status_id ListId,
                             twc_string Slug,
                             twc_lists_subscribers_create_params Params);

/* lists/subscribers/destroy:
Unsubscribes the authenticated user from the specified list. */
typedef twc_param_struct {
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_subscribers_destroy_params;

extern twc_call_result
twc_Lists_Subscribers_Destroy(twc_state* Twitter,
                              twc_status_id ListId,
                              twc_string Slug,
                              twc_lists_subscribers_destroy_params Params);

/* lists/update:
Updates the specified list. The authenticated user must own the list to be able to update it. */
typedef twc_param_struct {
    /* The name for the list. */
    twc_string$ Name;
    /* Whether your list is public or private. Values can be public or private . If
no mode is specified the list will be public. */
    twc_string$ Mode;
    /* The description to give the list. */
    twc_string$ Description;
    /* The screen name of the user who owns the list being requested by a slug . */
    twc_string$ OwnerScreenName;
    /* The user ID of the user who owns the list being requested by a slug . */
    twc_status_id$ OwnerId;
} twc_lists_update_params;

extern twc_call_result
twc_Lists_Update(twc_state* Twitter,
                 twc_status_id ListId,
                 twc_string Slug,
                 twc_lists_update_params Params);

/* mutes/users/create:
Mutes the user specified in the ID parameter for the authenticating user. Returns the muted user in the requested format when successful. Returns a string describing the failure condition when unsuccessful. Actions taken in this method are asynchronous and changes will be eventually consistent. */
typedef twc_param_struct {
    /* The screen name of the potentially muted user. Helpful for disambiguating when a
valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The ID of the potentially muted user. Helpful for disambiguating when a valid
user ID is also a valid screen name. */
    twc_user_id$ UserId;
} twc_mutes_users_create_params;

extern twc_call_result
twc_Mutes_Users_Create(twc_state* Twitter,
                       twc_mutes_users_create_params Params);

/* mutes/users/destroy:
Un-mutes the user specified in the ID parameter for the authenticating user. Returns the unmuted user in the requested format when successful. Returns a string describing the failure condition when unsuccessful. Actions taken in this method are asynchronous and changes will be eventually consistent. */
typedef twc_param_struct {
    /* The screen name of the potentially muted user. Helpful for disambiguating when a
valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The ID of the potentially muted user. Helpful for disambiguating when a valid
user ID is also a valid screen name. */
    twc_user_id$ UserId;
} twc_mutes_users_destroy_params;

extern twc_call_result
twc_Mutes_Users_Destroy(twc_state* Twitter,
                        twc_mutes_users_destroy_params Params);

/* saved_searches/create:
Create a new saved search for the authenticated user. A user may only have 25 saved searches. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_savedsearches_create_params;

extern twc_call_result
twc_SavedSearches_Create(twc_state* Twitter,
                         twc_string Query,
                         twc_savedsearches_create_params Params);

/* saved_searches/destroy/:id:
Destroys a saved search for the authenticating user. The authenticating user must be the owner of saved search id being destroyed. */
typedef twc_param_struct {
    // No parameters documented as of generation
    void* Reserved;
} twc_savedsearches_destroy_id_params;

extern twc_call_result
twc_SavedSearches_Destroy_Id(twc_state* Twitter,
                             const char* Id,
                             twc_savedsearches_destroy_id_params Params);

/* statuses/destroy/:id:
Destroys the status specified by the required ID parameter. The authenticating user must be the author of the specified status. Returns the destroyed status if successful. */
typedef twc_param_struct {
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
} twc_statuses_destroy_id_params;

extern twc_call_result
twc_Statuses_Destroy_Id(twc_state* Twitter,
                        twc_status_id Id,
                        twc_statuses_destroy_id_params Params);

/* statuses/retweet/:id:
Retweets a tweet. Returns the original tweet with retweet details embedded. Usage Notes : */
typedef twc_param_struct {
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
} twc_statuses_retweet_id_params;

extern twc_call_result
twc_Statuses_Retweet_Id(twc_state* Twitter,
                        twc_status_id Id,
                        twc_statuses_retweet_id_params Params);

/* statuses/unretweet/:id:
Untweets a retweeted status. Returns the original
Tweet with retweet details
embedded. Usage Notes : */
typedef twc_param_struct {
    /* When set to either true , t or 1 , each tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
} twc_statuses_unretweet_id_params;

extern twc_call_result
twc_Statuses_Unretweet_Id(twc_state* Twitter,
                          twc_status_id Id,
                          twc_statuses_unretweet_id_params Params);

/* statuses/update:
Updates the authenticating user's current status, also known as Tweeting. For each update attempt, the update text is compared with the authenticating user's recent Tweets. Any attempt that would result in duplication will be blocked, resulting in a 403 error. A user cannot submit the same status twice in a row. While not rate limited by the API, a user is limited in the number of Tweets they can create at a time. If the number of updates posted by the user reaches the current allowed limit this method will return an HTTP 403 error. About Geo */
typedef twc_param_struct {
    /* The ID of an existing status that the update is in reply to. Note: This
parameter will be ignored unless the author of the Tweet this parameter
references is mentioned within the status text. Therefore, you must include
@username , where username is the author of the referenced Tweet, within the
update. */
    twc_status_id$ InReplyToStatusId;
    /* If you upload Tweet media that might be considered sensitive content such as
nudity, violence, or medical procedures, you should set this value to true. See
Media setting and best practices
for more context.
Defaults to false. */
    bool$ PossiblySensitive;
    /* The latitude of the location this Tweet refers to. This parameter will be
ignored unless it is inside the range -90.0 to +90.0 (North is positive)
inclusive. It will also be ignored if there isn't a corresponding long
parameter. */
    twc_string$ Lat;
    /* The longitude of the location this Tweet refers to. The valid ranges for
longitude is -180.0 to +180.0 (East is positive) inclusive. This parameter will
be ignored if outside that range, if it is not a number, if geo_enabled is
disabled, or if there not a corresponding lat parameter. */
    twc_string$ Long;
    /* A place in the world. */
    twc_place_id$ PlaceId;
    /* Whether or not to put a pin on the exact coordinates a Tweet has been sent from. */
    bool$ DisplayCoordinates;
    /* When set to either true , t or 1 , each Tweet returned in a timeline will
include a user object including only the status authors numerical ID. Omit this
parameter to receive the complete user object. */
    bool$ TrimUser;
    /* A list of media_ids to associate with the Tweet. You may include up to 4
photos or 1 animated GIF or 1 video in a Tweet. See Uploading Media
for further details on uploading media. */
    const char* MediaIds;
} twc_statuses_update_params;

extern twc_call_result
twc_Statuses_Update(twc_state* Twitter,
                    twc_string Status,
                    twc_statuses_update_params Params);

/* statuses/update_with_media:
This endpoint has been DEPRECATED and does not support multiple images, animated GIFs, or video. Follow the `Uploading media guide <rest/media/uploading-media>`__ to upload one or more media entities, and then use `POST statuses/update </rest/reference/post/statuses/update>`__ to attach them to a Tweet. Updates the authenticating user's current status and attaches media for upload. In other words, it creates a Tweet with a picture attached. Unlike POST statuses/update , this method expects raw multipart data. Your POST request's Content-Type should be set to multipart/form-data with the media[] parameter. The Tweet text will be rewritten to include the media URL(s), which will reduce the number of characters allowed in the Tweet text. If the URL(s) cannot be appended without text truncation, the tweet will be rejected and this method will return an HTTP 403 error. Users are limited to a specific daily media upload limit. Requests to this endpoint will return the following headers with information regarding the user's current media upload limits: If the user is over the daily media limit, this method will return an HTTP 403 error. In addition to media upload limits, the user is still limited in the number of statuses they can publish daily. If the user tries to exceed the number of updates allowed, this method will also return an HTTP 403 error, similar to POST statuses/update . OAuth is handled differently for POST messages. See Uploading Media for more details on this. */
typedef twc_param_struct {
    /* Set to true for content which may not be suitable for every audience. */
    twc_string$ PossiblySensitive;
    /* The ID of an existing status that the update is in reply to. Note : This
parameter will be ignored unless the author of the tweet this parameter
references is mentioned within the status text. Therefore, you must include
@username , where username is the author of the referenced tweet, within the
update. */
    twc_status_id$ InReplyToStatusId;
    /* The latitude of the location this tweet refers to. This parameter will be
ignored unless it is inside the range -90.0 to +90.0 (North is positive)
inclusive. It will also be ignored if there isn't a corresponding long
parameter. */
    twc_string$ Lat;
    /* The longitude of the location this tweet refers to. The valid ranges for
longitude is -180.0 to +180.0 (East is positive) inclusive. This parameter will
be ignored if outside that range, not a number, geo_enabled is disabled, or
if there not a corresponding lat parameter. */
    twc_string$ Long;
    /* A place in the world identified by a Twitter place ID. Place IDs can be
retrieved from geo/reverse_geocode. */
    twc_place_id$ PlaceId;
    /* Whether or not to put a pin on the exact coordinates a tweet has been sent from. */
    bool$ DisplayCoordinates;
} twc_statuses_updatewithmedia_params;

extern twc_call_result
twc_Statuses_UpdateWithMedia(twc_state* Twitter,
                             twc_string Status,
                             twc_string Media,
                             twc_statuses_updatewithmedia_params Params);

/* users/report_spam:
Report the specified user as a spam account to Twitter. Additionally performs the equivalent of POST blocks / create on behalf of the authenticated user. */
typedef twc_param_struct {
    /* The ID or screen_name of the user you want to report as a spammer. Helpful for
disambiguating when a valid screen name is also a user ID. */
    twc_string$ ScreenName;
    /* The ID of the user you want to report as a spammer. Helpful for disambiguating
when a valid user ID is also a valid screen name. */
    twc_user_id$ UserId;
} twc_users_reportspam_params;

extern twc_call_result
twc_Users_ReportSpam(twc_state* Twitter,
                     twc_users_reportspam_params Params);

