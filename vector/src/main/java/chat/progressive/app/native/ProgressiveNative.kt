package chat.progressive.app.native

import org.json.JSONObject
import org.json.JSONArray
import timber.log.Timber

/**
 * JNI bridge to the progressive_native C++ library.
 * Handles date validation, MSC3030 URL construction, and response parsing.
 */
object ProgressiveNative {

    private var isLoaded = false

    fun ensureLoaded() {
        if (!isLoaded) {
            try {
                System.loadLibrary("progressive_native")
                isLoaded = true
                Timber.d("progressive_native loaded successfully")
            } catch (e: UnsatisfiedLinkError) {
                Timber.e(e, "Failed to load progressive_native")
                // Fallback: use pure Kotlin implementation
            }
        }
    }

    /**
     * Validates the date string (YYYY-MM-DD) and builds the MSC3030 URL.
     * Gates on the feature flag from C++ side.
     *
     * @param roomId Target room ID
     * @param dateString Date in YYYY-MM-DD format
     * @param serverUrl Base URL of the homeserver
     * @param accessToken User's access token
     * @param isEnabled Whether jumptodate labs setting is enabled
     *
     * @return JSON string with url, accessToken, timestamp — or {"error":"..."} on failure
     */
    @JvmStatic
    external fun nativeValidateAndBuild(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): String

    /**
     * Parses the UTC timestamp_to_event response from the homeserver.
     *
     * @param responseBody HTTP response body
     * @param httpStatus HTTP status code
     *
     * @return JSON string with eventId — or error info on failure
     */
    @JvmStatic
    external fun nativeParseResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    /**
     * Parses a Matrix event JSON to find the source event it relates to
     * (reply, reaction, edit, thread root).
     *
     * @param eventJson Full JSON of the Matrix event
     * @param allowedTypes Comma-separated list of allowed relation types, or empty for all
     *
     * @return JSON with sourceEventId and relationType, or {"isRelation": false}
     */
    @JvmStatic
    external fun nativeParseRelation(
        eventJson: String,
        allowedTypes: String
    ): String

    // --- Export functions ---

    @JvmStatic
    external fun nativeFormatEventHtml(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        mediaSize: String,
        relationType: String,
        isContinuation: Boolean
    ): String

    @JvmStatic
    external fun nativeFormatEventPlainText(
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        fileName: String,
        relationType: String
    ): String

    @JvmStatic
    external fun nativeBuildHtmlExport(
        roomName: String,
        roomTopic: String,
        exportDate: String,
        eventHtmls: Array<String>
    ): String

    // --- Event Cache ---

    @JvmStatic
    external fun nativeCachePut(
        eventId: String,
        senderId: String,
        senderName: String,
        timestamp: String,
        body: String,
        msgType: String,
        eventType: String,
        relationType: String,
        sourceEventId: String,
        sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeCacheGetContext(eventId: String): String

    @JvmStatic
    external fun nativeCacheClear()

    @JvmStatic
    external fun nativeCacheSize(): Int

    // --- SQLite Event Database ---

    @JvmStatic
    external fun nativeDbOpen(dbPath: String): Boolean

    @JvmStatic
    external fun nativeDbClose()

    @JvmStatic
    external fun nativeDbInsertEvent(
        eventId: String, roomId: String, senderId: String, senderName: String,
        timestamp: String, body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String,
        originServerTs: Long, displayIndex: Int, sentByMe: Boolean
    )

    @JvmStatic
    external fun nativeDbGetContext(eventId: String): String

    @JvmStatic
    external fun nativeDbClearRoom(roomId: String)

    @JvmStatic
    external fun nativeDbCount(): Int

    // --- Native SQLite DB (SqliteDB - richer API with room summaries, transactions) ---

    @JvmStatic external fun nativeSqliteDbOpen(dbPath: String, key: String): Boolean
    @JvmStatic external fun nativeSqliteDbClose(key: String)
    @JvmStatic external fun nativeSqliteDbInsertEvent(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int
    ): Boolean
    @JvmStatic external fun nativeSqliteDbInsertEventRel(
        key: String, eventId: String, roomId: String, type: String, senderId: String,
        contentJson: String, originTs: Long, ageTs: Long, displayIndex: Int,
        stateKey: String, redacts: String, relType: String, relatesToId: String
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryEvents(key: String, roomId: String, limit: Int, offset: Int, ascending: Boolean): String
    @JvmStatic external fun nativeSqliteDbQueryEvent(key: String, eventId: String): String
    @JvmStatic external fun nativeSqliteDbDeleteEvent(key: String, eventId: String)
    @JvmStatic external fun nativeSqliteDbCountEvents(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbMaxDisplayIndex(key: String, roomId: String): Int
    @JvmStatic external fun nativeSqliteDbUpsertRoom(
        key: String, roomId: String, displayName: String, avatarUrl: String,
        topic: String, membership: String, notifCount: Int, highlightCount: Int,
        lastActivityMs: Long, isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean
    ): Boolean
    @JvmStatic external fun nativeSqliteDbQueryRooms(key: String): String
    @JvmStatic external fun nativeSqliteDbBeginTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbCommitTransaction(key: String)
    @JvmStatic external fun nativeSqliteDbSchemaVersion(key: String): Int

    // --- Translation ---

    @JvmStatic
    external fun nativeBuildTranslateRequest(
        text: String,
        sourceLang: String,
        targetLang: String,
        apiEndpoint: String,
        apiToken: String,
        model: String
    ): String

    @JvmStatic
    external fun nativeParseTranslateResponse(
        responseBody: String,
        httpStatus: Int
    ): String

    // --- Proxy / Tor / I2P ---

    @JvmStatic
    external fun nativeComputeProxyConfig(
        connType: Int,
        proxyType: Int,
        host: String,
        port: Int,
        username: String,
        password: String
    ): String

    // --- Yggdrasil ---

    @JvmStatic external fun nativeIsYggdrasilAddress(addr: String): Boolean
    @JvmStatic external fun nativeIsYggdrasilDomain(host: String): Boolean
    @JvmStatic external fun nativeBuildYggHomeserverUrl(addr: String, port: Int, tls: Boolean): String
    @JvmStatic external fun nativeRewriteHomeserverUrl(originalUrl: String, yggAddr: String): String

    // --- Markdown ---

    @JvmStatic external fun nativeMarkdownToHtml(markdown: String, enableTables: Boolean): String
    @JvmStatic external fun nativeParseMarkdownTable(tableBlock: String, withScroll: Boolean): String

    // --- Account Export ---

    @JvmStatic external fun nativeEncryptAccount(
        userId: String, token: String, refreshToken: String,
        homeServer: String, deviceId: String, deviceName: String,
        displayName: String, avatarUrl: String,
        includeCache: Boolean, passphrase: String
    ): String

    @JvmStatic external fun nativeDecryptAccount(encrypted: String, passphrase: String): String

    // --- Audio ---

    @JvmStatic external fun nativeFormatDuration(ms: Long): String
    @JvmStatic external fun nativeFormatPositionInfo(posMs: Long, durMs: Long): String
    @JvmStatic external fun nativeComputeProgress(posMs: Long, durMs: Long): Float
    @JvmStatic external fun nativeIsSupportedAudio(mimeType: String): Boolean

    // --- Media Filter ---

    @JvmStatic external fun nativeGetFileExtension(fileName: String, mimeType: String): String
    @JvmStatic external fun nativeIsValidMxcUri(uri: String): Boolean

    // --- Content Filter ---

    @JvmStatic external fun nativeKeywordFilterLoad(raw: String)
    @JvmStatic external fun nativeKeywordFilterCheck(text: String): String
    @JvmStatic external fun nativeKeywordFilterExport(): String
    @JvmStatic external fun nativeKeywordFilterCount(): Int
    @JvmStatic external fun nativeKeywordFilterClear()

    @JvmStatic external fun nativeShouldBlockImage(
        blockAll: Boolean, allowAvatars: Boolean, allowStickers: Boolean, allowEmoji: Boolean,
        mxcUrl: String, imageType: String
    ): Boolean

    // --- Network Stats ---

    @JvmStatic external fun nativeNetStatsStart(url: String, method: String): Int
    @JvmStatic external fun nativeNetStatsEnd(requestId: Int, statusCode: Int, bytesSent: Long, bytesReceived: Long, error: String)
    @JvmStatic external fun nativeNetStatsToJson(): String
    @JvmStatic external fun nativeNetStatsToText(): String
    @JvmStatic external fun nativeNetStatsClear()

    // --- Masquerade ---

    @JvmStatic external fun nativeIsValidMasqueradeName(name: String): Boolean
    @JvmStatic external fun nativeGetSuggestedMasqueradeNames(): String
    @JvmStatic external fun nativeIsValidIconAlias(alias: String): Boolean
    @JvmStatic external fun nativeBuildMasqueradeAlias(baseAlias: String, iconName: String): String

    // --- User Mask ---

    @JvmStatic external fun nativeUserMaskSet(mxid: String, displayName: String, avatarUrl: String, overrideMxid: String)
    @JvmStatic external fun nativeUserMaskRemove(mxid: String)
    @JvmStatic external fun nativeUserMaskResolveName(mxid: String, originalName: String): String
    @JvmStatic external fun nativeUserMaskResolveAvatar(mxid: String, originalUrl: String): String
    @JvmStatic external fun nativeUserMaskExportJson(): String
    @JvmStatic external fun nativeUserMaskImportJson(json: String)
    @JvmStatic external fun nativeIsValidMxid(mxid: String): Boolean
    @JvmStatic external fun nativeUserMaskClear()
    @JvmStatic external fun nativeUserMaskCount(): Int

    // --- Chunked Upload ---

    @JvmStatic external fun nativeUploaderSetChunkSizeMb(mb: Int)
    @JvmStatic external fun nativeUploaderComputeChunks(fileSize: Long): Int
    @JvmStatic external fun nativeUploaderGetChunkJson(index: Int): String
    @JvmStatic external fun nativeUploaderAdvance()
    @JvmStatic external fun nativeUploaderCancel()
    @JvmStatic external fun nativeUploaderFail(error: String)
    @JvmStatic external fun nativeUploaderReset()
    @JvmStatic external fun nativeUploaderProgressJson(): String
    @JvmStatic external fun nativeUploaderContentRange(chunkIndex: Int): String
    @JvmStatic external fun nativeSuggestChunkSizeMb(fileSize: Long): Int

    // --- Chat Features (Timezone + EXIF) ---

    @JvmStatic external fun nativeGetCommonTimezones(): String
    @JvmStatic external fun nativeFormatTimestampInTimezone(utcMs: Long, tzId: String): String
    @JvmStatic external fun nativeIsValidTimezoneId(tzId: String): Boolean
    @JvmStatic external fun nativeFileHasMetadata(mimeType: String): Boolean
    @JvmStatic external fun nativeGetStrippableMimeTypes(): String

    // --- Invitation Hide ---

    @JvmStatic external fun nativeInviteHide(roomId: String, roomName: String, inviterName: String, inviterMxid: String)
    @JvmStatic external fun nativeInviteUnhide(roomId: String)
    @JvmStatic external fun nativeInviteIsHidden(roomId: String): Boolean
    @JvmStatic external fun nativeInviteExportJson(): String
    @JvmStatic external fun nativeInviteImportJson(json: String)
    @JvmStatic external fun nativeInviteClear()
    @JvmStatic external fun nativeInviteCount(): Int

    // --- Thread Aggregator ---

    @JvmStatic external fun nativeThreadAdd(
        threadId: String, roomId: String, roomName: String,
        accountId: String, accountIndex: String,
        lastMsg: String, lastSender: String,
        lastTs: Long, replyCount: Int, unread: Boolean
    )
    @JvmStatic external fun nativeThreadGetAllJson(): String
    @JvmStatic external fun nativeThreadClear()
    @JvmStatic external fun nativeThreadCount(): Int
    @JvmStatic external fun nativeThreadRemoveRoom(roomId: String)

    // --- User Messages ---

    @JvmStatic external fun nativeFormatUserMessagePreview(roomName: String, body: String, msgType: String, maxLen: Int): String

    // --- Room Version ---

    @JvmStatic external fun nativeGetRoomVersionsJson(): String
    @JvmStatic external fun nativeIsValidRoomVersion(version: String): Boolean

    // --- Chat Preview ---

    @JvmStatic external fun nativeFormatShortTime(epochMs: Long): String
    @JvmStatic external fun nativeTruncateMessage(body: String, maxLen: Int): String

    // --- RAM Monitor ---

    @JvmStatic external fun nativeGetMemoryInfo(): String
    @JvmStatic external fun nativeFormatMemoryLabel(rssKb: Long): String

    // --- Cache Manager ---

    @JvmStatic external fun nativeCacheTrack(eventId: String, roomId: String, roomName: String, timestamp: Long, sizeBytes: Long, msgType: String, body: String)
    @JvmStatic external fun nativeCacheStatsJson(): String
    @JvmStatic external fun nativeCacheGetByRoom(roomId: String): String
    @JvmStatic external fun nativeCacheGetOlderThan(beforeTs: Long): String
    // --- Message Aggregator (All Messages) ---

    @JvmStatic external fun nativeMsgAggAdd(eventId: String, roomId: String, roomName: String, accountId: String, accountIndex: String, senderName: String, body: String, msgType: String, originServerTs: Long)
    @JvmStatic external fun nativeMsgAggGetAllJson(): String
    @JvmStatic external fun nativeMsgAggClear()
    @JvmStatic external fun nativeMsgAggCount(): Int

    // --- Room Info ---

    @JvmStatic external fun nativeFormatCreationDate(epochMs: Long): String
    @JvmStatic external fun nativeIsLikelyFullHistory(cached: Int, estimated: Int): Boolean

    // --- Deleted Archive ---

    @JvmStatic external fun nativeDeletedArchiveAdd(eventId: String, roomId: String, roomName: String, senderName: String, body: String, msgType: String, timestamp: String, originTs: Long, deletedBy: String)
    @JvmStatic external fun nativeDeletedArchiveExportJson(): String
    @JvmStatic external fun nativeDeletedArchiveClear()
    @JvmStatic external fun nativeDeletedArchiveCount(): Int

    // --- Search Index ---

    @JvmStatic external fun nativeSearchIndexMessage(eventId: String, roomId: String, roomName: String, senderName: String, body: String, timestamp: Long, isEncrypted: Boolean)
    @JvmStatic external fun nativeSearchQuery(query: String, limit: Int): String
    @JvmStatic external fun nativeSearchClear()
    @JvmStatic external fun nativeSearchIndexedCount(): Int

    // --- Module Loader ---

    @JvmStatic external fun nativeModuleScanDir(dirPath: String)
    @JvmStatic external fun nativeModuleEnable(name: String)
    @JvmStatic external fun nativeModuleIsEnabled(name: String): Boolean
    @JvmStatic external fun nativeModuleListJson(): String

    // --- Notification Keywords ---

    @JvmStatic external fun nativeNotifKeywordAdd(keyword: String, caseSensitive: Boolean)
    @JvmStatic external fun nativeNotifKeywordCheck(body: String): String
    @JvmStatic external fun nativeNotifKeywordExport(): String
    @JvmStatic external fun nativeNotifKeywordImport(json: String)
    @JvmStatic external fun nativeNotifKeywordClear()

    // --- Reaction Preview ---

    @JvmStatic external fun nativeFormatReactionPreview(reactorName: String, emoji: String, sourceBody: String, sourceSender: String): String

    // --- Room Mirror ---

    @JvmStatic external fun nativeMirrorAdd(srcRoomId: String, srcRoomName: String, mirRoomId: String, mirRoomName: String, enabled: Boolean, useDolls: Boolean)
    @JvmStatic external fun nativeMirrorRemove(srcRoomId: String)
    @JvmStatic external fun nativeMirrorIsActive(srcRoomId: String): Boolean
    @JvmStatic external fun nativeMirrorFormatMessage(senderName: String, senderMxid: String, roomName: String, body: String, msgType: String, ts: Long): String
    @JvmStatic external fun nativeMirrorGenerateDollMxid(originalMxid: String, targetServer: String): String
    @JvmStatic external fun nativeMirrorIsValidDoll(mxid: String): Boolean
    @JvmStatic external fun nativeMirrorExportJson(): String

    // --- Input Tools ---

    @JvmStatic external fun nativeSymbolAdd(symbol: String, label: String)
    @JvmStatic external fun nativeSymbolExport(): String
    @JvmStatic external fun nativeSymbolImport(json: String)

    @JvmStatic external fun nativeReplacementAddRule(pattern: String, replacement: String, exactMatch: Boolean)
    @JvmStatic external fun nativeReplacementApply(text: String): String
    @JvmStatic external fun nativeReplacementExport(): String

    // --- LLM ---

    @JvmStatic external fun nativeBuildLlmRequest(prompt: String, provider: Int, endpoint: String, token: String, model: String, systemPrompt: String, temp: Float, maxTokens: Int): String
    @JvmStatic external fun nativeBuildLlmHeaders(provider: Int, token: String): String
    @JvmStatic external fun nativeParseLlmResponse(body: String, statusCode: Int, provider: Int): String
    @JvmStatic external fun nativeFormatLlmBroadcast(prompt: String, response: String): String

    // --- Duplicate Names ---

    @JvmStatic external fun nativeFormatUserDisplayName(displayName: String, mxid: String, showMxid: Boolean): String

    // --- MXID Visibility ---

    @JvmStatic external fun nativeMxidVisibilityHide(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityShow(mxid: String)
    @JvmStatic external fun nativeMxidVisibilityIsVisible(mxid: String): Boolean
    @JvmStatic external fun nativeMxidVisibilityExport(): String

    // --- Read Receipts ---

    @JvmStatic external fun nativeComputeReceiptDisplay(receiptsJson: String, maxVisible: Int): String
    @JvmStatic external fun nativeFormatOverflowLabel(count: Int): String
    @JvmStatic external fun nativeFormatReceiptAccessibility(visibleJson: String, overflow: Int): String

    // --- Room Analytics ---

    @JvmStatic external fun nativeExtractServerName(mxid: String): String

    // --- User Hide Timer ---

    @JvmStatic external fun nativeUserHideFor(userId: String, displayName: String, minutes: Int)
    @JvmStatic external fun nativeUserHideIsHidden(userId: String): Boolean
    @JvmStatic external fun nativeUserHideGetActive(): String

    // --- Message Queue ---

    @JvmStatic external fun nativeMsgQueueEnqueue(msgId: String, roomId: String, body: String, formattedBody: String, order: Int, maxRetries: Int)
    @JvmStatic external fun nativeMsgQueueSetOrder(msgId: String, order: Int)
    @JvmStatic external fun nativeMsgQueueMarkFailed(msgId: String, error: String)
    @JvmStatic external fun nativeMsgQueueMarkSent(msgId: String)
    @JvmStatic external fun nativeMsgQueuePendingCount(): Int
    @JvmStatic external fun nativeMsgQueueExport(): String

    // --- Image Crop ---

    @JvmStatic external fun nativeIsValidCrop(imgW: Int, imgH: Int, x: Int, y: Int, w: Int, h: Int): Boolean

    // --- Auto-Scroll ---

    @JvmStatic external fun nativeComputeScrollPlan(smooth: Boolean, durationMin: Int, totalLines: Int, lineHeightPx: Int): String

    // --- Language Detection ---

    @JvmStatic external fun nativeDetectLanguage(text: String, method: Int): String
    @JvmStatic external fun nativeGetLanguageLabel(code: String): String

    // --- Language Hide ---

    @JvmStatic external fun nativeLangHideAdd(langCode: String, roomId: String, userId: String, specificUser: Boolean, minutes: Int)
    @JvmStatic external fun nativeLangHideIsHidden(langCode: String, roomId: String, userId: String): Boolean

    // --- Chat Push Down ---

    @JvmStatic external fun nativeChatPushDown(roomId: String, minutes: Int)
    @JvmStatic external fun nativeChatIsPushedDown(roomId: String): Boolean
    @JvmStatic external fun nativeChatPushDownRestore(roomId: String)

    // --- Emoji Blacklist ---

    @JvmStatic external fun nativeEmojiBlacklistAdd(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistRemove(emoji: String)
    @JvmStatic external fun nativeEmojiBlacklistIsBlocked(emoji: String): Boolean
    @JvmStatic external fun nativeEmojiBlacklistExport(): String
    @JvmStatic external fun nativeEmojiBlacklistImport(json: String)

    // --- Avatar History ---

    @JvmStatic external fun nativeAvatarAddChange(mxcUrl: String, eventId: String, timestamp: Long)
    @JvmStatic external fun nativeAvatarExportJson(): String
    @JvmStatic external fun nativeAvatarClear()

    // --- Jump to Date with Time ---

    @JvmStatic external fun nativeParseJumpToDate(input: String): String

    // --- Room Matching ---

    @JvmStatic external fun nativeMatchRooms(query: String, roomsJson: String): String
    @JvmStatic external fun nativeIsRoomId(input: String): Boolean
    @JvmStatic external fun nativeIsRoomAlias(input: String): Boolean

    // --- Event Links ---

    @JvmStatic external fun nativeExtractEventLinks(body: String): String
    @JvmStatic external fun nativeFormatResolvedEvent(sender: String, body: String): String
    @JvmStatic external fun nativeIsEventId(text: String): Boolean

    // --- Timestamps ---

    @JvmStatic external fun nativeFormatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatFullTimestamp(epochMs: Long): String

    // --- Lightweight Call ---

    @JvmStatic external fun nativeLightCallEnter(): String
    @JvmStatic external fun nativeLightCallExit(): String
    @JvmStatic external fun nativeLightCallAssessMemory(): String
    @JvmStatic external fun nativeShouldUseLightweightMode(): Boolean

    // --- Scheduled Edits ---

    @JvmStatic external fun nativeSchedEditSchedule(roomId: String, targetEventId: String, newContent: String, contentUrl: String, formattedContent: String, formattedUrl: String, scheduledAtMs: Long, recurring: Boolean): String
    @JvmStatic external fun nativeSchedEditCancel(editId: String)
    @JvmStatic external fun nativeSchedEditGetDue(): String
    @JvmStatic external fun nativeSchedEditMarkApplied(editId: String)
    @JvmStatic external fun nativeSchedEditMarkFailed(editId: String, error: String)
    @JvmStatic external fun nativeSchedEditExport(): String
    @JvmStatic external fun nativeSchedEditStats(): String

    // --- SVG Rendering ---

    @JvmStatic external fun nativeParseSvg(svgData: String): String
    @JvmStatic external fun nativeIsValidSvg(data: String): Boolean

    // --- Drawing Canvas ---

    @JvmStatic external fun nativeDrawMoveTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawLineTo(x: Double, y: Double)
    @JvmStatic external fun nativeDrawSetColor(argb: Int)
    @JvmStatic external fun nativeDrawSetWidth(w: Double)
    @JvmStatic external fun nativeDrawExportJson(): String
    @JvmStatic external fun nativeDrawToSvgPath(): String
    @JvmStatic external fun nativeDrawClear()

    // --- Profile Swiper ---

    @JvmStatic external fun nativeProfileSwiperSetProfiles(profilesJson: String)
    @JvmStatic external fun nativeProfileSwiperNext(): String
    @JvmStatic external fun nativeProfileSwiperPrev(): String

    // --- Rainbow Generator ---

    @JvmStatic external fun nativeGenerateRainbow(text: String): String

    // --- Text Formatting ---

    @JvmStatic external fun nativeFormatSpoiler(text: String): String
    @JvmStatic external fun nativeFormatEmote(sender: String, text: String): String
    @JvmStatic external fun nativeFormatShrug(text: String): String
    @JvmStatic external fun nativeFormatLenny(text: String): String
    @JvmStatic external fun nativeFormatTableFlip(text: String): String
    @JvmStatic external fun nativeFormatPlain(text: String): String
    @JvmStatic external fun nativeIsEmojiOnly(text: String): Boolean
    @JvmStatic external fun nativeTruncateText(text: String, maxLen: Int): String

    // --- URL Tools ---

    @JvmStatic external fun nativeParseUrl(url: String): String
    @JvmStatic external fun nativeExtractFirstUrl(text: String): String
    @JvmStatic external fun nativeUrlEncode(input: String): String
    @JvmStatic external fun nativeUrlDecode(input: String): String
    @JvmStatic external fun nativeBuildMatrixToUrl(roomId: String): String

    // --- Notification Priority ---

    @JvmStatic external fun nativeComputeNotifPriority(isDM: Boolean, isMention: Boolean, isRoomMention: Boolean, isKeyword: Boolean, isCall: Boolean, isBackground: Boolean, dnd: Boolean, favorite: Boolean): String
    @JvmStatic external fun nativeFormatNotifTitle(roomName: String, senderName: String, isDM: Boolean): String
    @JvmStatic external fun nativeFormatNotifBody(body: String, senderName: String, isDM: Boolean, showSender: Boolean): String
    @JvmStatic external fun nativeIsRoomMention(body: String): Boolean

    // --- Matrix Patterns ---

    @JvmStatic external fun nativeIsUserId(input: String): Boolean
    @JvmStatic external fun nativeParseMatrixToPermalink(url: String): String
    @JvmStatic external fun nativeIsValidEmail(input: String): Boolean

    // --- Desync Detector ---

    @JvmStatic external fun nativeDesyncTrackEvent(eventId: String, serverName: String, timestamp: Long)
    @JvmStatic external fun nativeDesyncCheck(roomId: String, currentServer: String): String

    // --- Latency Tracker ---

    @JvmStatic external fun nativeLatencyRecord(latencyMs: Double, server: String, endpoint: String, success: Boolean)
    @JvmStatic external fun nativeLatencyStats(): String
    @JvmStatic external fun nativeLatencyStatsText(): String

    // --- String Utils ---

    @JvmStatic external fun nativeSanitizeRoomName(input: String): String
    @JvmStatic external fun nativeWordCount(input: String): Int

    // --- Location Sharing ---

    @JvmStatic external fun nativeLocationStartSession(roomId: String, userId: String, intervalSec: Int, autoStop: Boolean, autoStopMin: Int): String
    @JvmStatic external fun nativeLocationStopSession(sessionId: String)
    @JvmStatic external fun nativeLocationIsDue(sessionId: String): Boolean
    @JvmStatic external fun nativeLocationFormatMessage(lat: Double, lon: Double, acc: Double, label: String): String
    @JvmStatic external fun nativeLocationFormatGeoJson(lat: Double, lon: Double, acc: Double): String
    @JvmStatic external fun nativeLocationDistance(lat1: Double, lon1: Double, lat2: Double, lon2: Double): Double

    // --- Color Utils ---

    @JvmStatic external fun nativeContrastRatio(fgR: Int, fgG: Int, fgB: Int, bgR: Int, bgG: Int, bgB: Int): Double
    @JvmStatic external fun nativeParseColor(input: String): String

    // --- E2EE Utils ---

    @JvmStatic external fun nativeGetTrustLabel(level: Int): String
    @JvmStatic external fun nativeGetTrustBadge(level: Int): String

    // --- Thumbnail ---

    @JvmStatic external fun nativeComputeThumbnail(srcW: Int, srcH: Int, maxW: Int, maxH: Int, upscale: Boolean, quality: Int): String
    @JvmStatic external fun nativeBuildThumbnailUrl(mxcUri: String, w: Int, h: Int, method: String, animated: Boolean): String

    // --- Waveform ---

    @JvmStatic external fun nativeSuggestBarCount(durationMs: Long): Int
    @JvmStatic external fun nativeComputeRmsVolume(samples: IntArray): Double

    // --- Session Timeout ---

    @JvmStatic external fun nativeShouldLock(lockMethod: Int, idleTimeoutMin: Int, maxSessionMin: Int, maxFailedPin: Int, lockOnBg: Boolean, lastActivityMs: Long, sessionStartMs: Long, failedAttempts: Int, isLocked: Boolean, isBackground: Boolean): Boolean
    @JvmStatic external fun nativeIsValidPin(pin: String, minLen: Int, maxLen: Int): Boolean

    // --- Password Validator ---

    @JvmStatic external fun nativeValidatePassword(password: String): String

    // --- Spellcheck ---

    @JvmStatic external fun nativeEditDistance(a: String, b: String): Int

    // --- Typing Indicator ---

    @JvmStatic external fun nativeFormatTypingText(namesJson: String): String

    // --- Hash Utils ---

    @JvmStatic external fun nativeSha256Hex(input: String): String
    @JvmStatic external fun nativeGenerateToken(bytes: Int): String

    // --- Room Stats ---

    @JvmStatic external fun nativeComputeMessagesPerDay(count: Int, firstTs: Long, lastTs: Long): Double

    // --- Mention Parser ---

    @JvmStatic external fun nativeBuildUserPill(userId: String, displayName: String): String
    @JvmStatic external fun nativeStripPills(html: String): String

    // --- Poll Utils ---

    @JvmStatic external fun nativeIsValidPollQuestion(question: String): Boolean

    // --- Reaction Utils ---

    @JvmStatic external fun nativeGetQuickReactions(): String

    // --- File Validator ---

    @JvmStatic external fun nativeFormatFileSize(bytes: Long): String
    @JvmStatic external fun nativeGetExtensionFromName(fileName: String): String

    // --- Date Utils ---

    @JvmStatic external fun nativeFormatChatTimestamp(epochMs: Long, includeSeconds: Boolean): String
    @JvmStatic external fun nativeFormatRelativeTime(epochMs: Long): String

    // --- Message Queue ---

    @JvmStatic external fun nativeTextSimilarity(a: String, b: String): Double

    // --- Pinned Events (Element Web parity) ---

    @JvmStatic external fun nativeParsePinnedEventIds(stateJson: String): String
    @JvmStatic external fun nativeBuildPinnedEventsContent(idsJson: String): String

    // --- Server Capabilities ---

    @JvmStatic external fun nativeParseServerCapabilities(wellKnownJson: String): String

    // --- Username Validator ---

    @JvmStatic external fun nativeValidateUsername(username: String): String

    // --- Emoji Analyzer ---

    @JvmStatic external fun nativeCountEmojis(text: String): Int
    @JvmStatic external fun nativeIsEmojiOnlyMessage(text: String): Boolean

    // --- Identity Utils ---

    @JvmStatic external fun nativeResolveMatrixId(input: String): String
    @JvmStatic external fun nativeGetInitials(name: String): String

    // --- Notification Analyzer ---

    @JvmStatic external fun nativeSuggestQuietHours(byHourJson: String): String

    // --- Sync Analyzer ---

    @JvmStatic external fun nativeFormatProgressBar(percent: Double, width: Int): String

    // --- User Rating ---

    @JvmStatic external fun nativeComputeStreak(timestampsJson: String): String

    // --- Event Timeline ---

    @JvmStatic external fun nativeFormatGroupLabel(timestampMs: Long): String

    // --- Room Directory ---

    @JvmStatic external fun nativeExtractServers(roomsJson: String): String

    // --- SSO Utils ---

    @JvmStatic external fun nativeValidateHomeserverUrl(input: String): String

    // --- Backup Utils ---

    @JvmStatic external fun nativeIsValidRecoveryKey(key: String): Boolean

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatDeviceLastSeen(lastSeenMs: Long): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Presence ---

    @JvmStatic external fun nativeFormatPresence(presence: Int): String
    @JvmStatic external fun nativeFormatPresenceLine(presence: Int, lastActiveAgoMs: Long, statusMsg: String): String

    // --- Room Permissions ---

    @JvmStatic external fun nativeGetSuggestedRole(plJson: String, userId: String): String

    // --- Room Summary ---

    @JvmStatic external fun nativeFormatLastMessagePreview(sender: String, body: String, encrypted: Boolean): String

    // --- Membership ---

    @JvmStatic external fun nativeFormatMembership(membershipStr: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeValidateEvent(eventId: String, eventType: String, senderId: String, contentJson: String, originTs: String, blockedUsersJson: String): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeIsRoomEncrypted(stateJson: String): Boolean

    // --- Login Utils ---

    @JvmStatic external fun nativeBuildLoginBody(userId: String, password: String, deviceName: String, deviceId: String): String

    // --- Account Utils ---

    @JvmStatic external fun nativeValidatePasswordWithUsername(password: String, username: String): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeConnMonitorOnConnected()
    @JvmStatic external fun nativeConnMonitorOnDisconnected()
    @JvmStatic external fun nativeConnMonitorGetStatus(): String

    // --- Push Rules ---

    @JvmStatic external fun nativeParsePushCondition(conditionJson: String): String

    // --- Space Utils ---

    @JvmStatic external fun nativeBuildSpaceChildContent(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String

    // --- Event Relations ---

    @JvmStatic external fun nativeParseEventRelation(contentJson: String): String

    // --- E2EE Decoration ---

    @JvmStatic external fun nativeComputeE2eeDecoration(encrypted: Boolean, verified: Boolean, crossSigned: Boolean, decryptError: Boolean, blacklisted: Boolean, beforeJoined: Boolean, errorReason: String): String

    // --- Room List ---

    @JvmStatic external fun nativeGetBadgeText(count: Int, highlights: Int): String

    // --- Media Utils ---

    @JvmStatic external fun nativeMimeToMsgType(mimeType: String): String
    @JvmStatic external fun nativeIsValidBlurhash(hash: String): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeFormatNotifMode(mode: Int): String

    // --- Invite Utils ---

    @JvmStatic external fun nativeBuildInviteBody(userId: String, reason: String): String

    // --- Verification ---

    @JvmStatic external fun nativeGetVerificationEmojis(): String

    // --- Session Manager ---

    @JvmStatic external fun nativeFormatSessionBadge(unread: Int, highlights: Int): String

    // --- Auth Utils ---

    @JvmStatic external fun nativeFormatRateLimitMessage(responseJson: String, httpStatus: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsServerNotice(contentJson: String): Boolean

    // --- Event Encryption ---

    @JvmStatic external fun nativeParseEncryptedHeader(contentJson: String): String

    // --- Report Utils ---

    @JvmStatic external fun nativeGetReasonDescription(code: String): String

    // --- Secret Storage ---

    @JvmStatic external fun nativeExtractDefaultSecretKey(accountDataJson: String): String
    @JvmStatic external fun nativeHasCrossSigningSecrets(accountDataJson: String): Boolean

    // --- Report / Rageshake ---

    @JvmStatic external fun nativeIsOffensive(score: Int): Boolean
    @JvmStatic external fun nativeTruncateReportDescription(description: String, maxLen: Int): String

    // --- Content Scanner ---

    @JvmStatic external fun nativeIsContentScannerAvailable(serverCapabilitiesJson: String): Boolean

    // --- Matrix Error ---

    @JvmStatic external fun nativeIsPasswordError(errorCode: String): Boolean
    @JvmStatic external fun nativeGetAllErrorCodes(): String

    // --- Call Content Builders ---

    @JvmStatic external fun nativeBuildCallInviteContent(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String
    @JvmStatic external fun nativeBuildCallAnswerContent(callId: String, sdpAnswer: String): String
    @JvmStatic external fun nativeGetCallState(eventContentJson: String): String

    // --- Megolm Decryptor ---

    @JvmStatic external fun nativeMegolmAddSession(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean
    @JvmStatic external fun nativeMegolmDecrypt(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeMegolmSessionCount(): Int
    @JvmStatic external fun nativeMegolmClearRoom(roomId: String)

    // --- Olm Account & Session ---

    @JvmStatic external fun nativeOlmCreateAccount(userId: String, deviceId: String): Boolean
    @JvmStatic external fun nativeOlmGetIdentityKeys(): String
    @JvmStatic external fun nativeOlmGenerateOneTimeKeys(count: Int): String
    @JvmStatic external fun nativeOlmSignMessage(message: String): String
    @JvmStatic external fun nativeOlmCreateInboundSession(theirIdentityKey: String, preKeyMessage: String): String
    @JvmStatic external fun nativeOlmDecryptMessage(senderKey: String, sessionId: String, ciphertext: String): String
    @JvmStatic external fun nativeOlmPickleAccount(): String
    @JvmStatic external fun nativeOlmUnpickleAccount(pickled: String, userId: String, deviceId: String): Boolean

    // --- Device Verification ---

    @JvmStatic external fun nativeVerifyDeviceSignature(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean
    @JvmStatic external fun nativeComputeDeviceFingerprint(identityKeyBase64: String): String

    // --- SAS Emoji Verification ---

    @JvmStatic external fun nativeSasCreate(): String
    @JvmStatic external fun nativeSasSetTheirKey(theirPubkey: String): Boolean
    @JvmStatic external fun nativeSasGetEmojis(): String
    @JvmStatic external fun nativeSasCalculateMac(input: String, info: String): String
    @JvmStatic external fun nativeSasVerifyMac(theirMac: String, input: String, info: String): Boolean
    @JvmStatic external fun nativeSasDestroy()

    // --- WebRTC Utils ---

    @JvmStatic external fun nativeFormatCallDuration(seconds: Int): String
    @JvmStatic external fun nativeIsCallEvent(eventType: String): Boolean

    // --- Message Retry ---

    @JvmStatic external fun nativeComputeRetryDelay(retryCount: Int, maxDelayMs: Int): Int
    @JvmStatic external fun nativeDecideRetry(retryCount: Int, errorCode: Int, retryAfterHeader: String): String
    @JvmStatic external fun nativeFormatMessageStatus(state: Int): String

    // --- Sync Utils ---

    @JvmStatic external fun nativeBuildSyncFilter(includeThreads: Boolean, includePresence: Boolean, timelineLimit: Int): String

    // --- Event Display ---

    @JvmStatic external fun nativeClassifyEvent(eventType: String, msgType: String): Int

    // --- Permalink ---

    @JvmStatic external fun nativeBuildEventPermalink(roomId: String, eventId: String): String

    // --- Network Monitor ---

    @JvmStatic external fun nativeGetRecommendedMediaQuality(type: Int, connected: Boolean, metered: Boolean, signal: Int, latency: Double, loss: Double): String

    // --- Client Info ---

    @JvmStatic external fun nativeCompareSemver(a: String, b: String): Int

    // --- Keyshare ---

    @JvmStatic external fun nativeBuildKeyRequestBody(roomId: String, sessionId: String, senderKey: String, algorithm: String, requestId: String, deviceId: String): String

    // --- Display Name ---

    @JvmStatic external fun nativeUserIdToDisplayName(userId: String): String
    @JvmStatic external fun nativeUserIdToColor(userId: String): String

    // --- Message Location ---

    @JvmStatic external fun nativeEstimatePaginationRequests(missingEvents: Int, pageSize: Int): Int

    // --- Timeline Utils ---

    @JvmStatic external fun nativeShouldAutoScroll(isOwnMessage: Boolean): Boolean

    // --- Cross Signing ---

    @JvmStatic external fun nativeParseCrossSigningStatus(accountDataJson: String, userId: String): String

    // --- Edit History ---

    @JvmStatic external fun nativeGetEditBadgeText(editCount: Int): String
    @JvmStatic external fun nativeGetEditCountBadge(editCount: Int): String
    @JvmStatic external fun nativeComputeEditDiffSummary(oldBody: String, newBody: String): String

    // --- Read Marker / Unread Count ---
    // Ported from: TimelineViewModel.kt (read marker index math)
    //              ReadMarkers.kt (server-side read marker management)
    //              RoomSummary.kt (unread count display)

    /**
     * Compute the read marker position and unread statistics.
     * @return JSON with lastReadEventId, firstUnreadEventId, unreadCount,
     *         unreadMentions, hasUnread, readMarkerIndex
     */
    @JvmStatic external fun nativeComputeReadMarker(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): String

    @JvmStatic external fun nativeShouldShowJumpToUnread(readMarkerJson: String): Boolean

    @JvmStatic external fun nativeFormatUnreadJumpLabel(readMarkerJson: String): String

    @JvmStatic external fun nativeAdvanceReadMarker(roomId: String, latestEventId: String): String
    @JvmStatic external fun nativeReadMarkerToJson(lastReadEventId: String, unreadCount: Int, unreadMentions: Int, unreadHighlights: Int, hasUnread: Boolean): String

    // --- Kotlin fallbacks for Read Marker ---

    fun computeReadMarkerFallback(
        lastReadEventId: String,
        loadedEventIds: Array<String>,
        loadedSenders: Array<String>,
        isMention: BooleanArray,
        isHighlight: BooleanArray,
        myUserId: String
    ): JSONObject {
        val result = JSONObject()
        result.put("lastReadEventId", lastReadEventId)
        result.put("firstUnreadEventId", "")
        result.put("unreadCount", 0)
        result.put("unreadMentions", 0)
        result.put("hasUnread", false)
        result.put("readMarkerIndex", -1)

        if (lastReadEventId.isEmpty() || loadedEventIds.isEmpty()) return result

        val markerIndex = loadedEventIds.indexOf(lastReadEventId)
        if (markerIndex < 0) return result

        result.put("readMarkerIndex", markerIndex)

        var unreadCount = loadedEventIds.size - markerIndex - 1
        var unreadMentions = 0
        for (i in markerIndex + 1 until loadedEventIds.size) {
            if (i < loadedSenders.size && loadedSenders[i] == myUserId) {
                unreadCount--
                continue
            }
            if (i < isMention.size && isMention[i]) unreadMentions++
        }

        result.put("unreadCount", maxOf(0, unreadCount))
        result.put("unreadMentions", unreadMentions)
        result.put("hasUnread", unreadCount > 0)
        if (markerIndex + 1 < loadedEventIds.size) {
            result.put("firstUnreadEventId", loadedEventIds[markerIndex + 1])
        }
        return result
    }

    fun shouldShowJumpToUnreadFallback(readMarkerJson: String): Boolean {
        return readMarkerJson.contains("\"hasUnread\": true") ||
               readMarkerJson.contains("\"hasUnread\":true")
    }

    fun formatUnreadJumpLabelFallback(unreadCount: Int, unreadMentions: Int): String {
        val sb = StringBuilder()
        sb.append(unreadCount)
        if (unreadCount == 1) sb.append(" new message")
        else sb.append(" new messages")
        if (unreadMentions > 0) sb.append(" ($unreadMentions mentions)")
        return sb.toString()
    }

    fun advanceReadMarkerFallback(roomId: String, latestEventId: String): String {
        return latestEventId
    }

    // --- Slash Commands ---
    // Ported from: SlashCommandParser.kt, SlashCommand.kt, Command.kt

    @JvmStatic external fun nativeParseSlashCommand(text: String): String
    @JvmStatic external fun nativeFormatSlashCommand(command: String, arguments: String, type: Int, senderDisplayName: String): String
    @JvmStatic external fun nativeIsKnownSlashCommand(text: String): Boolean
    @JvmStatic external fun nativeGetAvailableCommands(): String

    // --- Typing Monitor ---
    // Ported from: TypingUsersTracker.kt, TypingHelper.kt, TypingView.kt

    @JvmStatic external fun nativeUpdateTypingState(roomId: String, typingUserIds: Array<String>, displayNames: Array<String>, nowMs: Long): String
    @JvmStatic external fun nativeIsUserTyping(typingStateJson: String, userId: String, nowMs: Long): Boolean

    // --- URL Preview / OpenGraph ---
    // Ported from: UrlPreviewer.kt, EventHtmlRenderer.kt

    @JvmStatic external fun nativeParseUrlPreview(html: String, baseUrl: String): String
    @JvmStatic external fun nativeExtractHtmlTitle(html: String): String
    @JvmStatic external fun nativeResolveUrl(baseUrl: String, relative: String): String
    @JvmStatic external fun nativeStripHtmlTags(html: String): String

    // --- Power Levels / Room Permissions ---
    // Ported from: PowerLevelsContent.kt, RoomPermissions.kt, PowerLevelsHelper.kt

    @JvmStatic external fun nativeParsePowerLevels(json: String): String
    @JvmStatic external fun nativeComputeUserPermissions(powerLevelsJson: String, userId: String): String
    @JvmStatic external fun nativeGetUserRole(powerLevel: Int): String

    // --- Well-Known / Server Discovery ---
    // Ported from: WellKnown.kt, LoginServerUrlFormatter.kt

    @JvmStatic external fun nativeParseWellKnown(json: String): String
    @JvmStatic external fun nativeFormatServerUrl(input: String): String
    @JvmStatic external fun nativeIsValidHomeserverUrl(url: String): Boolean
    @JvmStatic external fun nativeNeedsWellKnownLookup(input: String): Boolean

    // --- Room Sort / Ordering ---
    // Ported from: RoomComparator.kt, RoomListViewModel.kt

    @JvmStatic external fun nativeSortRooms(roomsJson: String): String
    @JvmStatic external fun nativeGetRoomSortKey(lastEventTs: Long, notifCount: Int, highlightCount: Int, isDirect: Boolean, hasUnread: Boolean, tagStr: String, priority: Int): Int
    @JvmStatic external fun nativeGetRoomSectionName(tagStr: String, isDirect: Boolean): String

    // --- Key Backup / Recovery ---
    // Ported from: KeysBackup.kt, KeysBackupSetupSharedViewModel.kt

    @JvmStatic external fun nativeFormatRecoveryKey(raw: String): String
    @JvmStatic external fun nativeValidateRecoveryKey(key: String): Boolean
    @JvmStatic external fun nativeParseKeyBackupVersion(json: String): String
    @JvmStatic external fun nativeIsValidPassphrase(passphrase: String): Boolean
    @JvmStatic external fun nativeComputeRecoveryKey(curve25519Key: String): String
    @JvmStatic external fun nativeParseMatrixError(json: String): String
    @JvmStatic external fun nativeGetErrorDescription(errorCode: String): String
    @JvmStatic external fun nativeGetBackupAlgorithmDescription(algorithm: String): String
    @JvmStatic external fun nativeIsSupportedBackupAlgorithm(algorithm: String): Boolean
    @JvmStatic external fun nativeGetRecoveryKeyExample(): String
    @JvmStatic external fun nativeGetMinPassphraseLength(): Int

    // --- AI Agent Executor ---
    // Enables /agent <task> — LLM-driven chat automation like Claude Code

    @JvmStatic external fun nativeBuildAgentSystemPrompt(systemPrompt: String): String
    @JvmStatic external fun nativeProcessAgentIteration(stateJson: String, llmResponse: String): String
    @JvmStatic external fun nativeParseToolCalls(llmResponse: String): String
    @JvmStatic external fun nativeExtractTextAnswer(llmResponse: String): String

    // --- Push Condition Evaluator ---
    // Ported from: EventMatchCondition.kt, Glob.kt

    @JvmStatic external fun nativeEvaluatePushCondition(eventJson: String, key: String, pattern: String): Boolean
    @JvmStatic external fun nativeExtractJsonField(json: String, fieldPath: String): String

    // --- Thread Metadata ---
    // Computes header: title, starter avatar, message count

    @JvmStatic external fun nativeComputeThreadMeta(rootContent: String, replySenders: Array<String>, replyBodies: Array<String>, replyTimestamps: LongArray): String

    // --- Pending Message Editing ---
    // Edit messages before they're sent — no need to wait for server confirmation

    @JvmStatic external fun nativeCanEditPendingMessage(localId: String, state: Int): String

    // --- Sender Notification Filter ---
    // Per-room: choose which senders trigger notifications

    @JvmStatic external fun nativeShouldNotifyForSender(settingsJson: String, senderId: String, hasMention: Boolean, hasHighlight: Boolean): Boolean
    @JvmStatic external fun nativeToggleMuteSender(settingsJson: String, senderId: String, mute: Boolean): String

    // --- String Order / Fractional Indexing ---
    // Ported from: StringOrderUtils.kt, SpaceOrderUtils.kt

    @JvmStatic external fun nativeStringMidPoints(left: String, right: String, count: Int): String
    @JvmStatic external fun nativeStringAverage(left: String, right: String): String

    // --- Event Type Classifier ---
    // Ported from: EventType.kt (146L), MessageType.kt (52L)

    @JvmStatic external fun nativeRouteEventForProcessing(eventType: String, msgType: String): String
    @JvmStatic external fun nativeIsVerificationEvent(eventType: String): Boolean

    // --- Content Guard ---
    // Protects against emoji attacks and excessive media

    @JvmStatic external fun nativeCheckEmojiAttack(text: String, maxEmojis: Int, maxUnique: Int): String

    // --- Debug Tools ---

    @JvmStatic external fun nativeGetModuleCount(): String

    // --- User Status (like Element Web) ---

    @JvmStatic external fun nativeParseUserStatus(accountDataJson: String): String
    @JvmStatic external fun nativeBuildUserStatusJson(status: String, emoji: String): String

    // --- Jump to Unread Time Label ---
    // Optional: shows \"Jump to unread (3 hours ago)\" instead of just \"Jump to unread\"

    @JvmStatic external fun nativeFormatJumpToUnreadLabel(readMarkerJson: String, nowMs: Long): String

    // --- Content Utils (MXC URLs, Message Types) ---
    // Ported from: ContentUrlResolver.kt, MessageContent.kt

    @JvmStatic external fun nativeResolveMxcDownloadUrl(mxcUrl: String, homeServerUrl: String): String
    @JvmStatic external fun nativeParseMessageContent(contentJson: String): String
    @JvmStatic external fun nativeIsMxcUri(url: String): Boolean

    // --- Room State Parsers ---
    // Ported from: RoomJoinRules.kt, RoomHistoryVisibility.kt, RoomGuestAccess.kt, RoomCreate.kt

    @JvmStatic external fun nativeParseJoinRules(contentJson: String): String
    @JvmStatic external fun nativeParseHistoryVisibility(contentJson: String): String
    @JvmStatic external fun nativeParseGuestAccess(contentJson: String): String
    @JvmStatic external fun nativeParseRoomCreate(contentJson: String): String
    @JvmStatic external fun nativeParseTombstone(contentJson: String): String

    // --- Login Flow Parser ---
    // Ported from: LoginWizard.kt, LoginFlow.kt, LoginFlowTypes.kt

    @JvmStatic external fun nativeParseLoginFlows(json: String): String
    @JvmStatic external fun nativeGetLoginFlowDescription(type: String): String
    @JvmStatic external fun nativeGetSsoProviderIcon(providerId: String): String

    // --- Device Naming / User Agent ---
    // Ported from: ComputeUserAgentUseCase.kt

    @JvmStatic external fun nativeBuildUserAgent(appName: String, appVersion: String, manufacturer: String, model: String, androidVersion: String, buildId: String, flavor: String, sdkVersion: String): String
    @JvmStatic external fun nativeBuildDeviceDisplayName(appName: String, deviceModel: String): String
    @JvmStatic external fun nativeShortDeviceName(manufacturer: String, model: String): String

    // --- Sync Filter Builder ---
    // Ported from: SyncFilterParams.kt, SyncFilterBuilder.kt

    @JvmStatic external fun nativeGetDefaultSyncFilter(): String

    // --- Server Capabilities (updated from HomeServerCapabilities.kt) ---

    @JvmStatic external fun nativeIsDelegatedOidcEnabled(authenticationIssuer: String): Boolean

    // --- Room Display Name ---
    // Ported from: RoomDisplayNameResolver.kt

    @JvmStatic external fun nativeParseRoomNameContent(contentJson: String): String
    @JvmStatic external fun nativeNormalizeRoomName(name: String): String

    // --- Notification Count Formatter ---
    // Ported from: RoomSummaryFormatter.kt

    @JvmStatic external fun nativeFormatUnreadCounter(count: Int): String
    @JvmStatic external fun nativeFormatBadgeText(totalCount: Int): String

    // --- Permalink Parser (updated) ---

    @JvmStatic external fun nativeParsePermalinkFull(url: String): String

    // --- Kotlin fallbacks for Slash Commands ---

    fun parseSlashCommandFallback(text: String): JSONObject {
        val result = JSONObject()
        result.put("isSlashCommand", false)
        result.put("command", "")
        result.put("arguments", "")
        result.put("type", -1)
        result.put("needsMatrixId", false)
        result.put("isMessage", false)
        result.put("isAdmin", false)
        if (text.isEmpty() || !text.startsWith("/")) return result

        val spaceIdx = text.indexOf(' ')
        val cmdPart = if (spaceIdx < 0) text else text.substring(0, spaceIdx)
        val args = if (spaceIdx >= 0) text.substring(spaceIdx + 1).trim() else ""
        val lowered = cmdPart.lowercase()

        val commands = mapOf(
            "/me" to 1, "/join" to 2, "/part" to 3, "/invite" to 4,
            "/kick" to 5, "/ban" to 6, "/unban" to 7, "/op" to 8,
            "/deop" to 9, "/nick" to 10, "/topic" to 11, "/roomname" to 12,
            "/notice" to 13, "/shrug" to 14, "/tableflip" to 15, "/unflip" to 16,
            "/lenny" to 17, "/rainbow" to 18, "/rainbowme" to 19, "/plain" to 20,
            "/spoiler" to 21, "/avatar" to 22, "/discardsession" to 23,
            "/clearscalartoken" to 24, "/markdown" to 25
        )
        val type = commands[lowered] ?: return result

        result.put("isSlashCommand", true)
        result.put("command", cmdPart)
        result.put("arguments", args)
        result.put("type", type)
        result.put("isMessage", type in listOf(1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 25))
        result.put("isAdmin", type in listOf(5, 6, 7, 8, 9))
        return result
    }

    fun formatSlashCommandFallback(command: String, arguments: String, type: Int, sender: String): String {
        return when (type) {
            1, 19 -> "* $sender $arguments"
            13 -> arguments
            14 -> if (arguments.isNotEmpty()) "$arguments ¯\\_(ツ)_/¯" else "¯\\_(ツ)_/¯"
            15 -> "(╯°□°）╯︵ ┻━┻"
            16 -> "┬──┬ ノ( ゜-゜ノ)"
            17 -> "( ͡° ͜ʖ ͡°)"
            21 -> "||$arguments||"
            else -> arguments
        }
    }

    // --- Room Counter (multi-account room counting, join-order numbering, export) ---

    @JvmStatic external fun nativeCountRooms(roomsJson: String, accountCount: Int, uniqueOnly: Boolean, perAccountSplit: Boolean): String
    @JvmStatic external fun nativeAssignJoinOrder(roomsJson: String, accountCount: Int): String
    @JvmStatic external fun nativeSwapAccountOrder(accountsJson: String, posA: Int, posB: Int): String
    @JvmStatic external fun nativeIsDumpBetter(candidateEventCount: Int, candidateStartMs: Long, candidateEndMs: Long, baselineEventCount: Int, baselineStartMs: Long, baselineEndMs: Long, candidateHasGaps: Boolean, baselineHasGaps: Boolean): Boolean
    @JvmStatic external fun nativePrioritizeExportServers(serversJson: String): String

    // --- Web Search ---

    @JvmStatic external fun nativeBuildSearchUrl(engine: String, endpoint: String, apiKey: String, engineId: String, query: String, maxResults: Int): String
    @JvmStatic external fun nativeParseSearchResponse(engine: String, responseJson: String, query: String): String
    @JvmStatic external fun nativeParseWebCommand(commandArgs: String): String
    @JvmStatic external fun nativeFormatSearchResultsForAgent(responseJson: String): String

    // --- Native HTTP Client (TLS bridge) ---

    @JvmStatic external fun nativeInitTlsBridge(): Boolean

    @JvmStatic fun nativeTlsRequest(host: String, port: Int, request: String, timeoutMs: Int): String {
        return tlsRequestFallback(host, port, request, timeoutMs)
    }

    // --- Native API (homeserver config) ---

    @JvmStatic external fun nativeSetHomeserverUrl(url: String)
    @JvmStatic external fun nativeSetAccessToken(token: String)
    @JvmStatic external fun nativeApiSync(filter: String, since: String, timeout: Int): String
    @JvmStatic external fun nativeApiLogin(userId: String, password: String, deviceId: String): String
    @JvmStatic external fun nativeApiSendEvent(roomId: String, eventType: String, txnId: String, contentJson: String): String
    @JvmStatic external fun nativeApiJoinRoom(roomId: String, reason: String): String
    @JvmStatic external fun nativeApiLeaveRoom(roomId: String): String
    @JvmStatic external fun nativeApiGetProfile(userId: String): String
    @JvmStatic external fun nativeApiWhoAmI(): String
    @JvmStatic external fun nativeApiLogout(): Boolean
    @JvmStatic external fun nativeApiGetRoomMembers(roomId: String): String
    @JvmStatic external fun nativeApiInviteUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiAvailable(): Boolean

    // --- Extended API (room, profile, search, moderation) ---

    @JvmStatic external fun nativeApiGetRoomMessages(roomId: String, from: String, dir: String, limit: Int): String
    @JvmStatic external fun nativeApiCreateRoom(name: String, topic: String, isDirect: Boolean, invitees: String): String
    @JvmStatic external fun nativeApiSearch(query: String, roomId: String, limit: Int): String
    @JvmStatic external fun nativeApiKickUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiBanUser(roomId: String, userId: String, reason: String): String
    @JvmStatic external fun nativeApiUnbanUser(roomId: String, userId: String): String
    @JvmStatic external fun nativeApiRedactEvent(roomId: String, eventId: String, txnId: String): String
    @JvmStatic external fun nativeApiGetPushRules(): String
    @JvmStatic external fun nativeApiCreateFilter(userId: String, filterJson: String): String
    @JvmStatic external fun nativeApiGetDisplayName(userId: String): String
    @JvmStatic external fun nativeApiSetDisplayName(userId: String, displayName: String): String
    @JvmStatic external fun nativeApiGetVersions(): String
    @JvmStatic external fun nativeApiLogoutAll(): Boolean
    @JvmStatic external fun nativeApiPublicRooms(server: String, query: String, limit: Int): String

    // --- Display Name & Avatar Utilities ---


    // --- Permalink Builder ---

    @JvmStatic external fun nativeBuildRoomPermalink(roomId: String): String
    @JvmStatic external fun nativeBuildUserPermalink(userId: String): String

    // --- Media Utilities ---


    // --- Room Encryption ---


    // --- Event Display ---

    @JvmStatic external fun nativeShouldShowTimestamp(currentSender: String, currentTs: Long, previousTs: Long, showAll: Boolean): Boolean

    @JvmStatic external fun nativeGenerateDeviceName(model: String, osVersion: String): String

    // --- Account Validation ---

    @JvmStatic external fun nativeIsValidDisplayName(name: String, maxLen: Int): Boolean

    // --- Well-Known / Server Discovery ---

    @JvmStatic external fun nativeNeedsWellKnownDiscovery(homeserverUrl: String): Boolean

    // --- Polls ---

    @JvmStatic external fun nativeIsPollEnded(closeTimestampMs: Long): Boolean

    // --- Membership ---

    @JvmStatic external fun nativeCanReadMessages(membership: String): Boolean

    // --- Invites ---


    // --- Event Validation ---

    @JvmStatic external fun nativeIsBodyWithinLimits(body: String, maxLength: Int): Boolean

    // --- Widgets ---

    @JvmStatic external fun nativeIsJitsiWidget(type: String): Boolean
    @JvmStatic external fun nativeGetWidgetTypeName(type: String): String

    // --- WebRTC / Calls ---

    @JvmStatic external fun nativeIsCallExpired(createdAtMs: Long, timeoutSec: Int): Boolean

    // --- Notification Settings ---

    @JvmStatic external fun nativeParseNotifMode(action: String): String

    // --- Crypto / E2EE ---

    @JvmStatic external fun nativeIsValidDeviceKey(key: String): Boolean

    // --- Server Info ---

    @JvmStatic external fun nativeParseServerVersion(apiResponseJson: String): String

    // --- Time Formatting ---

    @JvmStatic external fun nativeFormatTimeAgoLabel(timestampMs: Long, nowMs: Long): String

    // --- Edit History ---

    @JvmStatic external fun nativeFormatEditSummary(originalBody: String, newBody: String): String

    // --- Cross-Signing ---

    @JvmStatic external fun nativeNeedsCrossSigningSetup(statusJson: String): Boolean
    @JvmStatic external fun nativeFormatCrossSigningStatus(statusJson: String): String

    // --- Event Type Display ---

    @JvmStatic external fun nativeGetEventTypeDescription(eventType: String, msgType: String): String
    @JvmStatic external fun nativeGetEventTypeIcon(eventType: String, msgType: String): String
    @JvmStatic external fun nativeIsContinuation(curSender: String, prevSender: String, curTs: Long, prevTs: Long): Boolean

    // --- Power Levels ---

    @JvmStatic external fun nativeParseRoomPowerLevels(stateContentJson: String): String
    @JvmStatic external fun nativeHasPower(plJson: String, userId: String, action: String): Boolean

    // --- SSO ---

    @JvmStatic external fun nativeIsSsoCallbackUrl(url: String): Boolean
    @JvmStatic external fun nativeExtractSsoProvider(idpId: String): String

    // --- Room Tombstone ---

    @JvmStatic external fun nativeParseRoomTombstoneContent(stateEventJson: String): String

    // --- Network Quality ---

    @JvmStatic external fun nativeClassifyNetworkQuality(signalStrength: Int, latencyMs: Double, lossRate: Double): String

    // --- Connection Monitor ---

    @JvmStatic external fun nativeFormatDowntime(downtimeMs: Long): String

    // --- Event Preview ---

    @JvmStatic external fun nativeFormatEventPreview(senderName: String, body: String, eventType: String, msgType: String, showSender: Boolean): String

    // --- Room Encryption ---

    @JvmStatic external fun nativeParseEncryptionConfig(stateContentJson: String): String
    @JvmStatic external fun nativeComputeEncryptionStatus(algorithm: String): String

    // --- Space Utilities ---

    @JvmStatic external fun nativeParseSpaceChildren(stateEventsJson: String): String

    // --- E2EE Decoration ---

    @JvmStatic external fun nativeGetE2eeIconName(state: String): String
    @JvmStatic external fun nativeGetE2eeColor(state: String): String

    // --- Backup Utilities ---

    @JvmStatic external fun nativeBuildCreateBackupBody(algorithm: String, authData: String): String
    @JvmStatic external fun nativeFormatBackupStats(infoJson: String): String
    @JvmStatic external fun nativeNeedsBackupAttention(infoJson: String): Boolean

    // --- Read Marker / Notifications ---

    @JvmStatic external fun nativeBuildRoomNotifSettingsBody(mode: String): String

    // --- Notifications / Badge ---


    // --- Presence ---

    @JvmStatic external fun nativeParsePresence(userId: String, apiResponseJson: String): String

    // --- Matrix Error ---

    @JvmStatic external fun nativeGetRetryAfterMs(errorJson: String): Long

    // --- OpenID Token ---

    @JvmStatic external fun nativeParseOpenIdToken(json: String): String

    // --- Notification Counts ---

    @JvmStatic external fun nativeFormatCombinedNotificationCount(roomCount: Int, threadCount: Int): String
    @JvmStatic external fun nativeGetTotalUnreadCount(roomCount: Int, threadCount: Int): Int

    // --- Presence Indicator ---

    @JvmStatic external fun nativeGetPresenceIndicator(presence: String): String
    @JvmStatic external fun nativeIsPresenceStale(lastUpdatedMs: Long): Boolean
    @JvmStatic external fun nativeFormatStatusMessage(message: String, maxLen: Int): String

    // --- Space Utilities ---

    @JvmStatic external fun nativeBuildSpaceParentContent(parentSpaceId: String, canonical: Boolean): String

    // --- Widget Utilities ---

    @JvmStatic external fun nativeParseWidgetStateContent(stateContentJson: String, widgetId: String, roomId: String): String

    // --- Key Share Requests ---

    @JvmStatic external fun nativeShouldShareKey(algorithm: String, hasSession: Boolean, sessionVerified: Boolean, userTrusted: Boolean): Boolean

    // --- Recovery Key ---

    @JvmStatic external fun nativeExtractCurveKeyFromRecoveryKey(recoveryKey: String): String

    // --- Membership ---

    @JvmStatic external fun nativeIsActiveMember(membership: String): Boolean

    // --- WebRTC / Calls ---

    @JvmStatic external fun nativeBuildCallHangupContent(callId: String, reason: String): String
    @JvmStatic external fun nativeFormatCallNotification(callJson: String): String

    // --- Content Scanner / ToS ---

    @JvmStatic external fun nativeMustAcceptTos(responseJson: String): Boolean
    @JvmStatic external fun nativeBuildTosAcceptBody(version: String): String

    // --- Login Utilities ---

    @JvmStatic external fun nativeIsValidLoginCredentials(userId: String, password: String): Boolean
    @JvmStatic external fun nativeGenerateDeviceId(): String

    // --- Password Validator ---

    @JvmStatic external fun nativeComputePasswordStrength(password: String): Int
    @JvmStatic external fun nativeGetStrengthLabel(strength: Int): String
    @JvmStatic external fun nativeGeneratePasswordFeedback(password: String): String

    // --- Event Validator ---

    @JvmStatic external fun nativeIsValidEventId(eventId: String): Boolean
    @JvmStatic external fun nativeIsValidSenderId(senderId: String): Boolean
    @JvmStatic external fun nativeIsFileSizeWithinLimits(fileSize: Long, maxSizeBytes: Long): Boolean

    // --- Invite Utilities ---

    @JvmStatic external fun nativeIsInviteExpired(invitedAtMs: Long, maxAgeDays: Int): Boolean
    @JvmStatic external fun nativeBuildKnockBody(reason: String): String

    // --- Device Manager ---

    @JvmStatic external fun nativeFormatFingerprint(fingerprint: String): String
    @JvmStatic external fun nativeIsDeviceInactive(lastSeenMs: Long): Boolean

    // --- Version Compatibility ---

    @JvmStatic external fun nativeSatisfiesMinVersion(current: String, minimum: String): Boolean

    // --- Federation Version ---

    @JvmStatic external fun nativeParseFederationVersion(json: String): String

    // --- Report Utilities ---

    @JvmStatic external fun nativeIsValidReportReason(reason: String): Boolean

    // --- URL Preview ---

    @JvmStatic external fun nativeIsPreviewableUrl(url: String): Boolean
    @JvmStatic external fun nativeExtractUrls(text: String): String

    // --- Device Manager ---


    // --- Permalink Utilities ---

    @JvmStatic external fun nativeIsSameRoomPermalink(url1: String, url2: String): Boolean

    // --- Display Name (advanced) ---

    @JvmStatic external fun nativeGetBestDisplayName(displayName: String, userId: String): String
    @JvmStatic external fun nativeFormatMemberName(displayName: String, userId: String, powerLevel: Int, showBadge: Boolean): String

    // --- Identity Utilities ---

    @JvmStatic external fun nativeIsEmail(input: String): Boolean
    @JvmStatic external fun nativeIsMsisdn(input: String): Boolean
    @JvmStatic external fun nativeExtractAliasLocalpart(alias: String): String

    @JvmStatic external fun nativeIsImageUrl(url: String): Boolean

    // --- Permalink Parser ---

    @JvmStatic external fun nativeExtractRoomIdFromPermalink(url: String): String
    @JvmStatic external fun nativeExtractEventIdFromPermalink(url: String): String
    @JvmStatic external fun nativeExtractUserIdFromPermalink(url: String): String

    // --- URL Preview ---

    @JvmStatic external fun nativeTruncateDescription(text: String, maxLen: Int): String

    // --- Device Type ---

    @JvmStatic external fun nativeClassifyDeviceType(userAgent: String, clientName: String): String


    // --- Device Type ---


    // --- Knock Reason ---

    @JvmStatic external fun nativeFormatKnockReason(reason: String): String

    // --- Server Compatibility ---

    @JvmStatic external fun nativeIsServerCompatible(serverVersion: String, minRequired: String): Boolean

    // --- Device Fingerprint ---

    @JvmStatic external fun nativeExtractDeviceFingerprint(deviceId: String, keysJson: String): String

    // --- Event Distance ---

    @JvmStatic external fun nativeEventDistance(indexA: Int, indexB: Int): Int

    // --- Widget Validation ---

    @JvmStatic external fun nativeIsEtherpadWidget(type: String): Boolean
    @JvmStatic external fun nativeIsValidWidgetUrl(url: String): Boolean

    // --- Encryption Defaults ---

    @JvmStatic external fun nativeGetDefaultEncryptionAlgorithm(): String
    @JvmStatic external fun nativeRequiresDeviceVerification(algorithm: String): Boolean

    // --- Event Timestamp Validation ---

    @JvmStatic external fun nativeIsReasonableTimestamp(originServerTs: String, maxFutureMs: Long): Boolean

    // --- OIDC / MAS Authentication ---

    @JvmStatic external fun nativeDiscoverOidc(homeserverUrl: String): String
    @JvmStatic external fun nativeBuildOAuthUrl(clientId: String, redirectUri: String, state: String, codeChallenge: String, prompt: String): String
    @JvmStatic external fun nativeExchangeOidcCode(tokenEndpoint: String, clientId: String, redirectUri: String, code: String, codeVerifier: String): String
    @JvmStatic external fun nativeParseOAuthCallback(url: String, redirectUri: String): String
    @JvmStatic external fun nativeGenerateOAuthState(): String
    @JvmStatic external fun nativeGeneratePkce(): String

    // --- Native Sync Response Parser (bypass Moshi) ---
    // Controlled by Labs: SETTINGS_LABS_NATIVE_SYNC_PARSER

    @JvmStatic external fun nativeParseSyncResponse(json: String): String
    @JvmStatic external fun nativeGetNextBatch(json: String): String
    @JvmStatic external fun nativeParseSyncRoomsJson(json: String): String
    @JvmStatic external fun nativeParseEvent(json: String): String
    @JvmStatic external fun nativeParseTimeline(json: String): String
    @JvmStatic external fun nativeCountEventsInSync(json: String): Int
    @JvmStatic external fun nativeSyncResponseRoundtrip(json: String): String
    @JvmStatic external fun nativeExtractNextBatchLight(partialJson: String): String

    // --- Timeline Chunk (native pagination engine) ---

    @JvmStatic external fun nativeTimelineAddEvents(roomId: String, eventsJson: String, prevToken: String, nextToken: String, direction: Int): Int
    @JvmStatic external fun nativeTimelineGetEvents(roomId: String): String
    @JvmStatic external fun nativeTimelineGetEvent(eventId: String): String
    @JvmStatic external fun nativeTimelineClear(roomId: String)
    @JvmStatic external fun nativeTimelineGetReplies(eventId: String): String
    @JvmStatic external fun nativeTimelineGetLatestEdit(eventId: String): String
    @JvmStatic external fun nativeTimelineGetThreadEvents(rootEventId: String): String
    @JvmStatic external fun nativeTimelineChunkCount(roomId: String): Int
    @JvmStatic external fun nativeTimelineGetSnapshot(roomId: String, limit: Int, offset: Int): String
    @JvmStatic external fun nativeTimelineEventsAvailable(roomId: String, direction: Int): Int
    @JvmStatic external fun nativeTimelineAttachDb(roomId: String, dbKey: String): Boolean

    // --- Event Utilities ---

    @JvmStatic external fun nativeFormatEventSummary(eventType: String, msgType: String, senderName: String, body: String, membership: String, displayName: String, isRedacted: Boolean, isEncrypted: Boolean): String
    @JvmStatic external fun nativeFormatTypingIndicator(namesJson: String, maxNames: Int): String
    @JvmStatic external fun nativeCalculateCapabilities(userLevel: Int, eventsDefault: Int, stateDefault: Int, inviteLvl: Int, kickLvl: Int, banLvl: Int, redactLvl: Int, notifyLvl: Int): String

    // --- Content Builder ---

    @JvmStatic external fun nativeBuildTextContent(msgType: String, body: String, formattedBody: String): String
    @JvmStatic external fun nativeBuildImageContent(body: String, mxcUrl: String, w: Int, h: Int, sz: Long, mime: String): String
    @JvmStatic external fun nativeBuildFileContent(body: String, mxcUrl: String, fileName: String, sz: Long, mime: String): String
    @JvmStatic external fun nativeBuildRoomStateContent(eventType: String, value1: String, value2: String): String

    // --- Media Utilities ---


    // --- Account Data ---

    @JvmStatic external fun nativeParseDirectMessages(json: String): String
    @JvmStatic external fun nativeParseIgnoredUsers(json: String): String
    @JvmStatic external fun nativeAddBreadcrumb(currentJson: String, roomId: String): String
    @JvmStatic external fun nativeIsValidUserId(userId: String): Boolean
    @JvmStatic external fun nativeServerNameFromMxid(mxid: String): String

    // --- Relation Builder ---

    @JvmStatic external fun nativeBuildReplyRelation(eventId: String): String
    @JvmStatic external fun nativeBuildThreadRelation(rootId: String, latestId: String, fallingBack: Boolean): String
    @JvmStatic external fun nativeBuildEditRelation(eventId: String): String
    @JvmStatic external fun nativeBuildReactionRelation(eventId: String, key: String): String
    @JvmStatic external fun nativeWrapWithRelation(contentJson: String, relationJson: String): String

    // --- Live Draft ---

    @JvmStatic external fun nativeShouldAutoDraft(text: String, threshold: Int): Boolean
    @JvmStatic external fun nativeBuildDraftMessage(prefix: String, text: String): String
    @JvmStatic external fun nativeFinalizeDraftMessage(fullDraftText: String, prefix: String): String

    // --- Pure Kotlin fallback implementations ---

    fun validateAndBuildFallback(
        roomId: String,
        dateString: String,
        serverUrl: String,
        accessToken: String,
        isEnabled: Boolean
    ): JSONObject {
        val result = JSONObject()

        if (!isEnabled) {
            return result.put("error", "/jumptodate is disabled. Enable it in Settings → Labs.")
        }

        // Validate date format
        val dateRegex = Regex("^(\\d{4})-(\\d{2})-(\\d{2})$")
        val match = dateRegex.matchEntire(dateString)
            ?: return result.put("error", "Invalid date format. Use YYYY-MM-DD.")

        val year = match.groupValues[1].toInt()
        val month = match.groupValues[2].toInt()
        val day = match.groupValues[3].toInt()

        if (month < 1 || month > 12) return result.put("error", "Invalid month. Must be 01-12.")
        if (day < 1 || day > 31) return result.put("error", "Invalid day. Must be 01-31.")

        // Days per month validation
        val daysInMonth = intArrayOf(31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
        val isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)
        val maxDay = if (month == 2 && isLeapYear) 29 else daysInMonth[month - 1]

        if (day > maxDay) return result.put("error", "Invalid day for given month.")

        // Compute Unix timestamp (UTC, milliseconds) using Java's Calendar
        val cal = java.util.GregorianCalendar(java.util.TimeZone.getTimeZone("UTC"))
        cal.set(year, month - 1, day, 0, 0, 0)
        cal.set(java.util.Calendar.MILLISECOND, 0)
        val timestamp = cal.timeInMillis

        // Build MSC3030 URL
        val url = "$serverUrl/_matrix/client/unstable/org.matrix.msc3030/rooms/$roomId/timestamp_to_event?ts=$timestamp&dir=f"

        result.put("url", url)
        result.put("accessToken", accessToken)
        result.put("timestamp", timestamp)

        return result
    }

    fun parseResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()

        if (responseBody == null) {
            return result.put("error", "Empty response from server.")
                    .put("statusCode", httpStatus)
        }

        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val errcode = json.optString("errcode", "")
                val error = json.optString("error", "")
                val msg = buildString {
                    append("Server returned $httpStatus")
                    if (errcode.isNotEmpty()) append(" ($errcode)")
                    if (error.isNotEmpty()) append(": $error")
                }
                result.put("error", msg)
                result.put("statusCode", httpStatus)
                return result
            } catch (e: Exception) {
                return result.put("error", "Server returned $httpStatus")
                        .put("statusCode", httpStatus)
            }
        }

        try {
            val json = JSONObject(responseBody)
            val eventId = json.optString("event_id", "")

            if (eventId.isEmpty()) {
                return result.put("error", "Response missing event_id field.")
                        .put("statusCode", httpStatus)
            }

            if (eventId == "~") {
                return result.put("error", "No events found after the given date.")
                        .put("statusCode", httpStatus)
            }

            result.put("eventId", eventId)
            result.put("statusCode", httpStatus)
            return result
        } catch (e: Exception) {
            return result.put("error", "Failed to parse server response.")
                    .put("statusCode", httpStatus)
        }
    }

    fun parseRelationFallback(eventJson: String, allowedTypes: String): JSONObject {
        val result = JSONObject()
        try {
            val json = JSONObject(eventJson)
            val content = json.optJSONObject("content") ?: json
            val relatesTo = content.optJSONObject("m.relates_to")
                ?: content.optJSONObject("relates_to") ?: return result.put("isRelation", false)

            val sourceEventId = relatesTo.optString("event_id", "")
            val relationType = relatesTo.optString("rel_type", "")

            if (sourceEventId.isEmpty()) return result.put("isRelation", false)

            val jumpableTypes = setOf("m.annotation", "m.reference", "m.replace", "m.thread")
            if (allowedTypes.isNotEmpty() && !allowedTypes.split(",").any { it == relationType }) {
                return result.put("isRelation", false)
            }
            if (!jumpableTypes.contains(relationType)) {
                return result.put("isRelation", false)
            }

            result.put("isRelation", true)
            result.put("sourceEventId", sourceEventId)
            result.put("relationType", relationType)
            return result
        } catch (e: Exception) {
            return result.put("isRelation", false)
        }
    }

    // --- Export fallback implementations ---

    fun formatEventHtmlFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, mediaSize: String,
        relationType: String, isContinuation: Boolean
    ): String = buildString {
        if (!isContinuation) {
            appendLine("<div class=\"mx_EventTile\">")
            appendLine("  <div class=\"mx_EventTile_info\">")
            append("    <span class=\"mx_EventTile_sender\">").append(escapeHtmlFn(senderName)).appendLine("</span>")
            if (timestamp.isNotEmpty())
                append("    <span class=\"mx_MessageTimestamp\">").append(timestamp).appendLine("</span>")
            appendLine("  </div>")
        } else {
            appendLine("<div class=\"mx_EventTile mx_EventTile_continuation\">")
        }
        appendLine("  <div class=\"mx_EventTile_body\">")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            appendLine("    <div class=\"mx_EventTile_attachment\">")
            append("      <span class=\"mx_Attachment_name\">").append(fileName.ifEmpty { msgType.substring(2) }).appendLine("</span>")
            if (mediaSize.isNotEmpty())
                append("      <span class=\"mx_Attachment_size\">").append(mediaSize).appendLine(" bytes</span>")
            appendLine("    </div>")
        }
        if (body.isNotEmpty()) {
            appendLine("    <div class=\"mx_EventTile_content\">")
            append("      ").appendLine(escapeHtmlFn(body))
            appendLine("    </div>")
        }
        appendLine("  </div>")
        appendLine("</div>")
    }

    fun formatEventPlainTextFallback(
        senderName: String, timestamp: String, body: String,
        msgType: String, fileName: String, relationType: String
    ): String = buildString {
        if (timestamp.isNotEmpty()) append(timestamp).append(" - ")
        append(senderName).append(": ")
        if (msgType in listOf("m.image", "m.video", "m.file", "m.audio")) {
            append("[").append(msgType.substring(2)).append(" attached")
            if (fileName.isNotEmpty()) append(": ").append(fileName)
            append("]")
        }
        if (body.isNotEmpty()) append(" ").append(body)
        if (relationType == "m.reference") append(" (in reply)")
        appendLine()
    }

    fun buildHtmlExportFallback(
        roomName: String, roomTopic: String, exportDate: String, eventHtmls: Array<String>
    ): String = buildString {
        appendLine("<!DOCTYPE html>")
        appendLine("<html lang=\"en\">")
        appendLine("<head>")
        appendLine("<meta charset=\"UTF-8\">")
        append("<title>").append(escapeHtmlFn(roomName)).appendLine(" — Chat Export</title>")
        appendLine("<style>body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,sans-serif;margin:0;padding:16px;background:#f5f5f5;}.mx_EventTile{background:#fff;border-radius:8px;padding:12px 16px;margin-bottom:8px;box-shadow:0 1px 2px rgba(0,0,0,0.05);}.mx_EventTile_continuation{margin-top:-4px;border-radius:0 0 8px 8px;}.mx_EventTile_sender{font-weight:600;color:#333;margin-right:8px;}.mx_MessageTimestamp{color:#999;font-size:0.85em;}.mx_EventTile_body{color:#222;line-height:1.5;}.mx_EventTile_attachment{background:#f0f0f0;border-radius:4px;padding:8px;margin-bottom:8px;}.mx_ExportHeader{background:#fff;border-radius:8px;padding:16px;margin-bottom:16px;box-shadow:0 1px 3px rgba(0,0,0,0.1);}.mx_ExportHeader h1{margin:0 0 8px;font-size:1.5em;}.mx_ExportHeader p{margin:4px 0;color:#666;font-size:0.9em;}.mx_EventTile_content{white-space:pre-wrap;word-wrap:break-word;}.mx_EventTile_reaction{font-style:italic;color:#666;}</style>")
        appendLine("</head>")
        appendLine("<body>")
        appendLine("<div class=\"mx_ExportHeader\">")
        append("  <h1>").append(escapeHtmlFn(roomName)).appendLine("</h1>")
        if (roomTopic.isNotEmpty()) append("  <p>").append(escapeHtmlFn(roomTopic)).appendLine("</p>")
        append("  <p>Exported: ").append(exportDate).appendLine("</p>")
        append("  <p>Total messages: ").append(eventHtmls.size).appendLine("</p>")
        appendLine("</div>")
        for (html in eventHtmls) append(html)
        appendLine("<hr><p style=\"color:#999;text-align:center;\">Exported with Progressive Chat</p>")
        appendLine("</body></html>")
    }

    private fun escapeHtmlFn(s: String): String = s
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace("\"", "&quot;")

    // --- Kotlin fallback cache (in-memory HashMap) ---

    data class CachedEvent(
        val eventId: String = "",
        val senderId: String = "",
        val senderName: String = "",
        val timestamp: String = "",
        val body: String = "",
        val msgType: String = "",
        val eventType: String = "",
        val relationType: String = "",
        val sourceEventId: String = "",
        val sentByMe: Boolean = false
    )

    private val fallbackCache = mutableMapOf<String, CachedEvent>()
    private val fallbackRelationIndex = mutableMapOf<String, MutableList<String>>()

    fun cachePutFallback(
        eventId: String, senderId: String, senderName: String, timestamp: String,
        body: String, msgType: String, eventType: String,
        relationType: String, sourceEventId: String, sentByMe: Boolean
    ) {
        val event = CachedEvent(eventId, senderId, senderName, timestamp, body, msgType, eventType, relationType, sourceEventId, sentByMe)
        synchronized(fallbackCache) {
            val old = fallbackCache[eventId]
            if (old != null && old.sourceEventId.isNotEmpty()) {
                fallbackRelationIndex[old.sourceEventId]?.remove(eventId)
            }
            fallbackCache[eventId] = event
            if (sourceEventId.isNotEmpty()) {
                fallbackRelationIndex.getOrPut(sourceEventId) { mutableListOf() }.add(eventId)
            }
        }
    }

    fun cacheGetContextFallback(eventId: String): JSONObject {
        val result = JSONObject()
        synchronized(fallbackCache) {
            val e = fallbackCache[eventId] ?: return result.put("cached", false)
            result.put("cached", true)
            result.put("eventId", e.eventId)
            result.put("senderId", e.senderId)
            result.put("senderName", e.senderName)
            result.put("timestamp", e.timestamp)
            result.put("body", e.body)
            result.put("msgType", e.msgType)
            result.put("eventType", e.eventType)
            result.put("sentByMe", e.sentByMe)
            result.put("hasFailed", false)
        }
        return result
    }

    fun cacheClearFallback() {
        synchronized(fallbackCache) {
            fallbackCache.clear()
            fallbackRelationIndex.clear()
        }
    }

    fun cacheSizeFallback(): Int = synchronized(fallbackCache) { fallbackCache.size }

    // --- Translation fallback ---

    fun buildTranslateRequestFallback(
        text: String, sourceLang: String, targetLang: String,
        apiEndpoint: String, apiToken: String, model: String
    ): JSONObject {
        val body = JSONObject()
        body.put("model", model)
        val messages = org.json.JSONArray()
        val systemMsg = JSONObject().apply {
            put("role", "system")
            var prompt = "You are a translator. Translate the following text"
            if (sourceLang.isNotEmpty()) prompt += " from $sourceLang"
            prompt += " to $targetLang. Output ONLY the translation, nothing else."
            put("content", prompt)
        }
        messages.put(systemMsg)
        messages.put(JSONObject().apply {
            put("role", "user")
            put("content", text)
        })
        body.put("messages", messages)
        body.put("temperature", 0.1)
        return body
    }

    fun parseTranslateResponseFallback(responseBody: String?, httpStatus: Int): JSONObject {
        val result = JSONObject()
        if (responseBody == null) return result.put("success", false).put("error", "Empty response")
        if (httpStatus != 200) {
            try {
                val json = JSONObject(responseBody)
                val error = json.optJSONObject("error")
                val msg = error?.optString("message", "Server returned $httpStatus") ?: "Server returned $httpStatus"
                return result.put("success", false).put("error", msg).put("statusCode", httpStatus)
            } catch (e: Exception) {
                return result.put("success", false).put("error", "Server returned $httpStatus").put("statusCode", httpStatus)
            }
        }
        try {
            val json = JSONObject(responseBody)
            val choices = json.optJSONArray("choices")
            if (choices == null || choices.length() == 0)
                return result.put("success", false).put("error", "No choices in response")
            val message = choices.getJSONObject(0).optJSONObject("message")
            if (message == null)
                return result.put("success", false).put("error", "No message in response")
            val content = message.optString("content", "")
            if (content.isEmpty())
                return result.put("success", false).put("error", "No content in response")
            result.put("success", true)
            result.put("translatedText", content)
            return result
        } catch (e: Exception) {
            return result.put("success", false).put("error", "Failed to parse response")
        }
    }

    // --- Room Counter fallbacks ---

    @JvmStatic fun countRoomsFallback(roomsJson: String, accountCount: Int, uniqueOnly: Boolean, perAccountSplit: Boolean): String {
        try {
            val rooms = JSONArray(roomsJson)
            val counts = IntArray(accountCount)
            for (i in 0 until rooms.length()) {
                val r = rooms.getJSONObject(i)
                val acc = r.optInt("accountIndex", 0)
                if (acc in 0 until accountCount) counts[acc]++
            }
            val total = counts.sum()
            if (perAccountSplit) return counts.joinToString("+")
            return "($total)"
        } catch (e: Exception) { return "(0)" }
    }

    @JvmStatic fun assignJoinOrderFallback(roomsJson: String, accountCount: Int): String {
        try {
            val rooms = JSONArray(roomsJson)
            val counters = IntArray(accountCount)
            var global = 0
            for (i in 0 until rooms.length()) {
                val r = rooms.getJSONObject(i)
                val acc = r.optInt("accountIndex", 0)
                if (acc in 0 until accountCount) counters[acc]++
                r.put("joinOrder", if (acc in 0 until accountCount) counters[acc] else 0)
                global++
                r.put("globalJoinOrder", global)
            }
            return rooms.toString()
        } catch (e: Exception) { return "[]" }
    }

    @JvmStatic fun swapAccountOrderFallback(accountsJson: String, posA: Int, posB: Int): String {
        try {
            val arr = JSONArray(accountsJson)
            if (posA in 0 until arr.length() && posB in 0 until arr.length()) {
                val tmp = arr.getJSONObject(posA).optInt("orderIndex", 0)
                arr.getJSONObject(posA).put("orderIndex", arr.getJSONObject(posB).optInt("orderIndex", 0))
                arr.getJSONObject(posB).put("orderIndex", tmp)
            }
            return arr.toString()
        } catch (e: Exception) { return accountsJson }
    }

    @JvmStatic fun isDumpBetterFallback(candidateEventCount: Int, candidateStartMs: Long, candidateEndMs: Long, baselineEventCount: Int, baselineStartMs: Long, baselineEndMs: Long, candidateHasGaps: Boolean, baselineHasGaps: Boolean): Boolean {
        if (candidateHasGaps && !baselineHasGaps) return false
        if (!candidateHasGaps || baselineHasGaps) {
            if (candidateEventCount > baselineEventCount) return true
            if (candidateEventCount == baselineEventCount && candidateStartMs <= baselineStartMs && candidateEndMs >= baselineEndMs) return true
        }
        return false
    }

    @JvmStatic fun prioritizeServersFallback(serversJson: String): String {
        try {
            val servers = JSONArray(serversJson)
            val active = mutableListOf<JSONObject>()
            for (i in 0 until servers.length()) {
                val s = servers.getJSONObject(i)
                if (!s.optBoolean("excluded", false)) active.add(s)
            }
            active.sortBy { it.optInt("priority", 0) }
            return JSONArray(active).toString()
        } catch (e: Exception) { return serversJson }
    }

    // --- Web Search fallbacks ---

    @JvmStatic fun buildSearchUrlFallback(engine: String, endpoint: String, apiKey: String, engineId: String, query: String, maxResults: Int): String {
        val q = query.replace(" ", "+")
        return when (engine) {
            "searxng" -> "${endpoint}search?q=${q}&format=json&pageno=1"
            "ddg" -> "https://api.duckduckgo.com/?q=${q}&format=json&no_html=1"
            "google" -> "https://customsearch.googleapis.com/customsearch/v1?key=${apiKey}&cx=${engineId}&q=${q}&num=${maxResults}"
            else -> ""
        }
    }

    @JvmStatic fun parseSearchResponseFallback(engine: String, json: String, query: String): String {
        try {
            val r = JSONObject(json)
            val result = JSONObject()
            result.put("query", query)
            val results = JSONArray()
            if (engine == "searxng") {
                val arr = r.optJSONArray("results") ?: return result.put("results", results).toString()
                for (i in 0 until arr.length()) {
                    val item = arr.getJSONObject(i)
                    val ri = JSONObject()
                    ri.put("title", item.optString("title"))
                    ri.put("url", item.optString("url"))
                    ri.put("snippet", item.optString("content"))
                    ri.put("source", item.optString("engine"))
                    results.put(ri)
                }
            } else if (engine == "google") {
                val arr = r.optJSONArray("items") ?: return result.put("results", results).toString()
                for (i in 0 until arr.length()) {
                    val item = arr.getJSONObject(i)
                    val ri = JSONObject()
                    ri.put("title", item.optString("title"))
                    ri.put("url", item.optString("link"))
                    ri.put("snippet", item.optString("snippet"))
                    ri.put("source", "google")
                    results.put(ri)
                }
            } else if (engine == "ddg") {
                val abstract = r.optString("Abstract")
                if (abstract.isNotEmpty()) {
                    val ri = JSONObject()
                    ri.put("title", r.optString("Heading", "Instant Answer"))
                    ri.put("url", r.optString("AbstractURL"))
                    ri.put("snippet", abstract)
                    ri.put("source", "duckduckgo")
                    results.put(ri)
                }
            }
            result.put("results", results)
            result.put("success", results.length() > 0)
            return result.toString()
        } catch (e: Exception) {
            return """{"query":"$query","results":[],"success":false,"error":"${e.message}"}"""
        }
    }

    @JvmStatic fun parseWebCommandFallback(args: String): String {
        val parts = args.trim().split(Regex("\\s+"), limit = 2)
        if (parts.isEmpty() || parts[0].isEmpty()) return """{"isValid":false}"""
        val engineWords = mapOf("ddg" to "ddg", "duckduckgo" to "ddg", "google" to "google", "g" to "google", "searxng" to "searxng", "sx" to "searxng")
        val engine = if (parts.size >= 2 && parts[0] in engineWords) engineWords[parts[0]] ?: "" else ""
        val query = if (engine.isNotEmpty() && parts.size >= 2) parts[1] else args.trim()
        return """{"engine":"$engine","query":"$query","isValid":${query.isNotEmpty()}}"""
    }

    @JvmStatic fun formatSearchForAgentFallback(responseJson: String): String {
        try {
            val r = JSONObject(responseJson)
            val sb = StringBuilder()
            sb.append("Web results for '${r.optString("query")}':\n")
            val results = r.optJSONArray("results") ?: return sb.toString()
            for (i in 0 until results.length()) {
                val item = results.getJSONObject(i)
                sb.append("${i+1}. ${item.optString("title")}\n")
                sb.append("   ${item.optString("url")}\n")
                val snippet = item.optString("snippet")
                if (snippet.isNotEmpty()) sb.append("   $snippet\n")
            }
            return sb.toString()
        } catch (e: Exception) { return "" }
    }

    // --- Live Draft fallbacks ---

    @JvmStatic fun shouldAutoDraftFallback(text: String, threshold: Int): Boolean {
        if (text.length < threshold) return false
        return text.contains(" ") || text.contains("\t") || text.contains("\n")
    }

    @JvmStatic fun buildDraftMessageFallback(prefix: String, text: String): String = "$prefix$text"

    @JvmStatic fun finalizeDraftFallback(full: String, prefix: String): String {
        return if (full.startsWith(prefix)) full.removePrefix(prefix) else full
    }

    // --- TLS Request Fallback (called from C++ via JNI) ---

    @JvmStatic fun tlsRequestFallback(host: String, port: Int, request: String, timeoutMs: Int): String {
        try {
            val socket = javax.net.ssl.SSLSocketFactory.getDefault().createSocket(host, port) as javax.net.ssl.SSLSocket
            socket.soTimeout = timeoutMs
            socket.startHandshake()

            val out = socket.outputStream
            out.write(request.toByteArray(Charsets.UTF_8))
            out.flush()

            val input = socket.inputStream
            val response = input.bufferedReader().readText()
            socket.close()
            return response
        } catch (e: Exception) {
            return ""
        }
    }

    // --- Native HTTP client initialization ---

    @JvmStatic fun initTlsBridge(): Boolean {
        return try {
            System.loadLibrary("progressive_native")
            true
        } catch (e: UnsatisfiedLinkError) {
            false
        }
    }

    // --- OIDC fallbacks ---

    @JvmStatic fun discoverOidcFallback(homeserverUrl: String): String {
        return """{"supportsOidc":false,"supportsPassword":true,"errorMessage":"Native OIDC not loaded"}"""
    }

    @JvmStatic fun buildOAuthUrlFallback(clientId: String, redirectUri: String, state: String, codeChallenge: String, prompt: String): String {
        return ""
    }

    @JvmStatic fun exchangeOidcCodeFallback(tokenEndpoint: String, clientId: String, redirectUri: String, code: String, codeVerifier: String): String {
        return """{"success":false,"errorMessage":"Native OIDC not loaded"}"""
    }

    @JvmStatic fun parseOAuthCallbackFallback(url: String, redirectUri: String): String {
        return if (url.contains("error=access_denied")) """{"action":"go_back"}"""
               else if (url.contains("code=")) """{"action":"success","fullUrl":"$url"}"""
               else """{"action":"none"}"""
    }

    @JvmStatic fun generateOAuthStateFallback(): String {
        return java.util.UUID.randomUUID().toString().replace("-", "")
    }

    @JvmStatic fun generatePkceFallback(): String {
        val verifier = java.util.Base64.getUrlEncoder().withoutPadding()
            .encodeToString(java.util.UUID.randomUUID().toString().toByteArray())
            .take(64)
        return """{"codeVerifier":"$verifier","codeChallenge":"$verifier"}"""
    }

    // --- Timeline Chunk fallbacks ---
    @JvmStatic fun timelineAddEventsFallback(roomId: String, eventsJson: String, prevToken: String, nextToken: String, direction: Int): Int = 0
    @JvmStatic fun timelineGetEventsFallback(roomId: String): String = "[]"
    @JvmStatic fun timelineGetEventFallback(eventId: String): String = "{}"
    @JvmStatic fun timelineClearFallback(roomId: String) {}
    @JvmStatic fun timelineGetRepliesFallback(eventId: String): String = "[]"
    @JvmStatic fun timelineGetLatestEditFallback(eventId: String): String = eventId
    @JvmStatic fun timelineGetThreadEventsFallback(rootEventId: String): String = "[]"
    @JvmStatic fun timelineChunkCountFallback(roomId: String): Int = 0
    @JvmStatic fun timelineGetSnapshotFallback(roomId: String, limit: Int, offset: Int): String = "[]"
    @JvmStatic fun timelineEventsAvailableFallback(roomId: String, direction: Int): Int = 0
    @JvmStatic fun timelineAttachDbFallback(roomId: String, dbKey: String): Boolean = false
    @JvmStatic fun timelineGetThreadEventsFallback(rootEventId: String): String = "[]"

    // --- Event Utils fallbacks ---
    @JvmStatic fun formatEventSummaryFallback(eventType: String, msgType: String, senderName: String, body: String, membership: String, displayName: String, isRedacted: Boolean, isEncrypted: Boolean): String {
        if (isRedacted) return "Message removed"
        return if (senderName.isNotEmpty()) "$senderName: $body" else body
    }
    @JvmStatic fun formatTypingIndicatorFallback(namesJson: String, maxNames: Int): String {
        val names = try { JSONArray(namesJson) } catch (e: Exception) { return "" }
        if (names.length() == 0) return ""
        if (names.length() == 1) return "${names.getString(0)} is typing..."
        return "${names.getString(0)} and ${names.length()-1} other(s) are typing..."
    }
    @JvmStatic fun calculateCapabilitiesFallback(userLevel: Int, eventsDefault: Int, stateDefault: Int, inviteLvl: Int, kickLvl: Int, banLvl: Int, redactLvl: Int, notifyLvl: Int): String {
        return """{"isOwner":${userLevel >= 100},"isModerator":${userLevel >= 50},"canSendMessages":${userLevel >= eventsDefault},"canInvite":${userLevel >= inviteLvl},"canKick":${userLevel >= kickLvl},"canBan":${userLevel >= banLvl}}"""
    }

    // --- Content Builder fallbacks ---
    @JvmStatic fun buildTextContentFallback(msgType: String, body: String, formattedBody: String): String {
        val fb = if (formattedBody.isNotEmpty()) ""","format":"org.matrix.custom.html","formatted_body":"$formattedBody"""" else ""
        return """{"msgtype":"$msgType","body":"$body"$fb}"""
    }
    @JvmStatic fun buildImageContentFallback(body: String, mxcUrl: String, w: Int, h: Int, sz: Long, mime: String): String {
        return """{"msgtype":"m.image","body":"$body","url":"$mxcUrl","info":{"w":$w,"h":$h,"size":$sz,"mimetype":"$mime"}}"""
    }
    @JvmStatic fun buildFileContentFallback(body: String, mxcUrl: String, fileName: String, sz: Long, mime: String): String {
        return """{"msgtype":"m.file","body":"$body","url":"$mxcUrl","filename":"$fileName","info":{"size":$sz,"mimetype":"$mime"}}"""
    }
    @JvmStatic fun buildRoomStateContentFallback(eventType: String, value1: String, value2: String): String {
        return when (eventType) {
            "m.room.name" -> """{"name":"$value1"}"""
            "m.room.topic" -> """{"topic":"$value1"}"""
            "m.room.member" -> """{"membership":"$value1","reason":"$value2"}"""
            else -> """{"value":"$value1"}"""
        }
    }

    // --- Media Utils fallbacks ---
    @JvmStatic fun calculateThumbnailSizeFallback(origW: Int, origH: Int, maxW: Int, maxH: Int): String = """{"width":$maxW,"height":$maxH}"""
    @JvmStatic fun formatFileSizeFallback(bytes: Long): String {
        if (bytes < 1024) return "$bytes B"
        val units = arrayOf("B", "KB", "MB", "GB")
        var size = bytes.toDouble(); var u = 0
        while (size >= 1024 && u < 3) { size /= 1024; u++ }
        return "%.1f %s".format(size, units[u])
    }
    @JvmStatic fun formatDurationFallback(ms: Long): String {
        val s = (ms / 1000).toInt()
        return if (s >= 3600) "%d:%02d:%02d".format(s/3600, s%3600/60, s%60)
               else "%d:%02d".format(s/60, s%60)
    }
    @JvmStatic fun sanitizeFilenameFallback(name: String, maxLen: Int): String = name.take(maxLen)

    // --- Account Data fallbacks ---
    @JvmStatic fun parseDirectMessagesFallback(json: String): String = json
    @JvmStatic fun parseIgnoredUsersFallback(json: String): String = "[]"
    @JvmStatic fun addBreadcrumbFallback(currentJson: String, roomId: String): String {
        return """{"recent_rooms":["$roomId"]}"""
    }
    @JvmStatic fun isValidUserIdFallback(userId: String): Boolean = userId.startsWith("@") && userId.contains(":")
    @JvmStatic fun serverNameFromMxidFallback(mxid: String): String = mxid.substringAfter(":")

    // --- Relation Builder fallbacks ---
    @JvmStatic fun buildReplyRelationFallback(eventId: String): String = """{"m.in_reply_to":{"event_id":"$eventId"}}"""
    @JvmStatic fun buildThreadRelationFallback(rootId: String, latestId: String, fallingBack: Boolean): String =
        """{"rel_type":"m.thread","event_id":"$rootId","m.in_reply_to":{"event_id":"$latestId"},"is_falling_back":$fallingBack}"""
    @JvmStatic fun buildEditRelationFallback(eventId: String): String = """{"rel_type":"m.replace","event_id":"$eventId"}"""
    @JvmStatic fun buildReactionRelationFallback(eventId: String, key: String): String = """{"rel_type":"m.annotation","event_id":"$eventId","key":"$key"}"""
    @JvmStatic fun wrapWithRelationFallback(contentJson: String, relationJson: String): String {
        return contentJson.dropLast(1) + ",\"m.relates_to\":" + relationJson + "}"
    }

    // --- Native Sync Response Parser fallbacks ---
    @JvmStatic fun nativeParseSyncResponseFallback(json: String): String =
        """{"next_batch":"","account_data_count":0,"presence_count":0,"to_device_count":0,"rooms_join_count":0,"rooms_invite_count":0,"rooms_leave_count":0,"device_lists_changed":0}"""
    @JvmStatic fun nativeGetNextBatchFallback(json: String): String = ""
    @JvmStatic fun nativeParseSyncRoomsJsonFallback(json: String): String = "[]"
    @JvmStatic fun nativeParseEventFallback(json: String): String = "{}"
    @JvmStatic fun nativeParseTimelineFallback(json: String): String =
        """{"events_count":0,"limited":false,"prev_batch":""}"""
    @JvmStatic fun nativeCountEventsInSyncFallback(json: String): Int = 0
    @JvmStatic fun nativeSyncResponseRoundtripFallback(json: String): String = json
    @JvmStatic fun nativeExtractNextBatchLightFallback(partialJson: String): String = ""

    // --- Native API fallbacks ---
    @JvmStatic fun nativeSetHomeserverUrlFallback(url: String) {}
    @JvmStatic fun nativeSetAccessTokenFallback(token: String) {}
    @JvmStatic fun nativeApiLoginFallback(userId: String, password: String, deviceId: String): String =
        """{"success":false,"error":"Native library not loaded"}"""
    @JvmStatic fun nativeApiSyncFallback(filter: String, since: String, timeout: Int): String =
        """{"next_batch":"","rooms_joined":0,"rooms_invited":0,"rooms_left":0,"events_total_timeline":0}"""
    @JvmStatic fun nativeApiSendEventFallback(roomId: String, eventType: String, txnId: String, contentJson: String): String = ""
    @JvmStatic fun nativeApiJoinRoomFallback(roomId: String, reason: String): String = ""
    @JvmStatic fun nativeApiLeaveRoomFallback(roomId: String): String = ""
    @JvmStatic fun nativeApiGetProfileFallback(userId: String): String = "{}"
    @JvmStatic fun nativeApiWhoAmIFallback(): String = """{"user_id":""}"""
    @JvmStatic fun nativeApiLogoutFallback(): Boolean = false
    @JvmStatic fun nativeApiGetRoomMembersFallback(roomId: String): String = """{"chunk":[]}"""
    @JvmStatic fun nativeApiInviteUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiAvailableFallback(): Boolean = false

    @JvmStatic fun nativeApiGetRoomMessagesFallback(roomId: String, from: String, dir: String, limit: Int): String = """{"chunk":[]}"""
    @JvmStatic fun nativeApiCreateRoomFallback(name: String, topic: String, isDirect: Boolean, invitees: String): String = ""
    @JvmStatic fun nativeApiSearchFallback(query: String, roomId: String, limit: Int): String = """{"search_categories":{}}"""
    @JvmStatic fun nativeApiKickUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiBanUserFallback(roomId: String, userId: String, reason: String): String = ""
    @JvmStatic fun nativeApiUnbanUserFallback(roomId: String, userId: String): String = ""
    @JvmStatic fun nativeApiRedactEventFallback(roomId: String, eventId: String, txnId: String): String = ""
    @JvmStatic fun nativeApiGetPushRulesFallback(): String = """{}"""
    @JvmStatic fun nativeApiCreateFilterFallback(userId: String, filterJson: String): String = ""
    @JvmStatic fun nativeApiGetDisplayNameFallback(userId: String): String = ""
    @JvmStatic fun nativeApiSetDisplayNameFallback(userId: String, displayName: String): String = ""
    @JvmStatic fun nativeApiGetVersionsFallback(): String = """{"versions":[]}"""
    @JvmStatic fun nativeApiLogoutAllFallback(): Boolean = false
    @JvmStatic fun nativeApiPublicRoomsFallback(server: String, query: String, limit: Int): String = """{"chunk":[]}"""

    // --- Display Name & Avatar fallbacks ---
    @JvmStatic fun nativeUserIdToDisplayNameFallback(userId: String, capitalize: Boolean): String {
        val name = userId.removePrefix("@").substringBefore(":")
        return if (capitalize) name.replaceFirstChar { it.uppercase() } else name
    }
    @JvmStatic fun nativeGetInitialsFallback(name: String, maxChars: Int): String = name.take(maxChars).uppercase()

    // --- Permalink fallbacks ---
    @JvmStatic fun nativeBuildEventPermalinkFallback(roomId: String, eventId: String): String = "https://matrix.to/#/$roomId/$eventId"
    @JvmStatic fun nativeBuildRoomPermalinkFallback(roomId: String): String = "https://matrix.to/#/$roomId"
    @JvmStatic fun nativeBuildUserPermalinkFallback(userId: String): String = "https://matrix.to/#/$userId"

    // --- Media fallbacks ---
    @JvmStatic fun nativeFormatFileSizeFallback(bytes: Long): String {
        if (bytes < 1024) return "$bytes B"
        val kb = bytes / 1024.0
        if (kb < 1024) return "%.1f KB".format(kb)
        return "%.1f MB".format(kb / 1024)
    }
    @JvmStatic fun nativeMimeToMsgTypeFallback(mimeType: String): String = when {
        mimeType.startsWith("image/") -> "m.image"
        mimeType.startsWith("video/") -> "m.video"
        mimeType.startsWith("audio/") -> "m.audio"
        else -> "m.file"
    }

    // --- Key Backup fallbacks ---
    @JvmStatic fun nativeFormatRecoveryKeyFallback(raw: String): String = raw.chunked(4).joinToString(" ")
    @JvmStatic fun nativeValidateRecoveryKeyFallback(key: String): Boolean = key.length == 59 && key.all { it in '0'..'9' || it in 'A'..'Z' || it in 'a'..'z' }
    @JvmStatic fun nativeGetBackupAlgorithmDescriptionFallback(algorithm: String): String = when(algorithm) {
        "m.megolm_backup.v1.curve25519-aes-sha2" -> "Encrypted backup (Megolm key)"
        else -> algorithm
    }
    @JvmStatic fun nativeIsSupportedBackupAlgorithmFallback(algorithm: String): Boolean = algorithm.startsWith("m.megolm_backup")
    @JvmStatic fun nativeGetRecoveryKeyExampleFallback(): String = "EsTj 4fGz 8hWq ... (example)"
    @JvmStatic fun nativeIsValidPassphraseFallback(passphrase: String): Boolean = passphrase.length >= 8
    @JvmStatic fun nativeGetMinPassphraseLengthFallback(): Int = 8

    // --- Room Encryption fallback ---
    @JvmStatic fun nativeIsRoomEncryptedFallback(stateContentJson: String): Boolean = stateContentJson.contains("m.megolm") || stateContentJson.contains("encrypted")

    // --- Event Display fallback ---
    @JvmStatic fun nativeShouldShowTimestampFallback(currentSender: String, currentTs: Long, previousTs: Long, showAll: Boolean): Boolean {
        if (showAll) return true
        return (currentTs - previousTs) > 300_000 // 5 minutes
    }

    // --- User ID fallback ---
    @JvmStatic fun nativeIsValidUserIdFallback(userId: String): Boolean = userId.startsWith("@") && userId.contains(":")

    // --- Date & Time fallbacks ---
    @JvmStatic fun nativeFormatDurationFallback(durationMs: Long): String {
        val sec = durationMs / 1000
        if (sec < 60) return "${sec}s"
        val min = sec / 60
        if (min < 60) return "${min}m ${sec % 60}s"
        return "${min / 60}h ${min % 60}m"
    }
    @JvmStatic fun nativeFormatPresenceFallback(presence: String, lastActiveMs: Long): String = when(presence) {
        "online" -> "Online"
        "unavailable" -> "Idle"
        else -> "Offline"
    }
    @JvmStatic fun nativeBuildDeviceDisplayNameFallback(appName: String, deviceModel: String): String = "$appName ($deviceModel)"
    @JvmStatic fun nativeGenerateDeviceNameFallback(model: String, osVersion: String): String = "Progressive Chat ($model, Android $osVersion)"
    @JvmStatic fun nativeIsValidDisplayNameFallback(name: String, maxLen: Int): Boolean = name.isNotEmpty() && name.length <= maxLen

    // --- Well-Known fallbacks ---
    @JvmStatic fun nativeParseWellKnownFallback(responseJson: String): String = """{"homeserver_url":"","identity_server":"","valid":false}"""
    @JvmStatic fun nativeNeedsWellKnownDiscoveryFallback(homeserverUrl: String): Boolean = !homeserverUrl.startsWith("https://matrix.")

    // --- Polls fallback ---
    @JvmStatic fun nativeIsPollEndedFallback(closeTimestampMs: Long): Boolean = closeTimestampMs > 0 && closeTimestampMs < System.currentTimeMillis()

    // --- Membership fallback ---
    @JvmStatic fun nativeCanReadMessagesFallback(membership: String): Boolean = membership == "join" || membership == "invite"

    // --- Invites fallback ---
    @JvmStatic fun nativeBuildInviteBodyFallback(userId: String, reason: String): String {
        val r = if (reason.isNotEmpty()) ""","reason":"$reason"""" else ""
        return """{"user_id":"$userId"$r}"""
    }

    // --- Event Validation fallback ---
    @JvmStatic fun nativeIsBodyWithinLimitsFallback(body: String, maxLength: Int): Boolean = body.length <= maxLength

    // --- Widgets fallback ---
    @JvmStatic fun nativeIsJitsiWidgetFallback(type: String): Boolean = type == "jitsi" || type == "m.jitsi"
    @JvmStatic fun nativeGetWidgetTypeNameFallback(type: String): String = when(type) {
        "jitsi", "m.jitsi" -> "Jitsi Meet"
        "etherpad", "m.etherpad" -> "Etherpad"
        else -> type
    }

    // --- WebRTC / Calls fallbacks ---
    @JvmStatic fun nativeIsCallExpiredFallback(createdAtMs: Long, timeoutSec: Int): Boolean =
        (System.currentTimeMillis() - createdAtMs) > timeoutSec * 1000
    @JvmStatic fun nativeFormatCallDurationFallback(seconds: Int): String {
        val m = seconds / 60; val s = seconds % 60
        return if (m > 0) "${m}m ${s}s" else "${s}s"
    }

    // --- Notification Settings fallbacks ---
    @JvmStatic fun nativeFormatNotifModeFallback(mode: String): String = when(mode) {
        "all" -> "All messages"
        "mentions" -> "Mentions only"
        "none" -> "Muted"
        else -> "Default"
    }
    @JvmStatic fun nativeParseNotifModeFallback(action: String): String = when {
        action.contains("notify") -> "all"
        action.contains("dont_notify") -> "none"
        else -> "mentions"
    }

    // --- Crypto fallback ---
    @JvmStatic fun nativeIsValidDeviceKeyFallback(key: String): Boolean =
        key.length in 43..44 && key.all { it in 'A'..'Z' || it in 'a'..'z' || it in '0'..'9' || it == '+' || it == '/' || it == '=' }

    // --- Server Info fallback ---
    @JvmStatic fun nativeParseServerVersionFallback(apiResponseJson: String): String = "0.0.0"

    // --- Time fallback ---
    @JvmStatic fun nativeFormatTimeAgoLabelFallback(timestampMs: Long, nowMs: Long): String {
        val diff = (nowMs - timestampMs) / 1000
        return when {
            diff < 60 -> "just now"
            diff < 3600 -> "${diff / 60}m ago"
            diff < 86400 -> "${diff / 3600}h ago"
            else -> "${diff / 86400}d ago"
        }
    }

    // --- Edit History fallbacks ---
    @JvmStatic fun nativeFormatEditSummaryFallback(originalBody: String, newBody: String): String = newBody
    @JvmStatic fun nativeGetEditBadgeTextFallback(editCount: Int): String = if (editCount > 0) "Edited ($editCount)" else ""

    // --- Cross-Signing fallbacks ---
    @JvmStatic fun nativeNeedsCrossSigningSetupFallback(statusJson: String): Boolean =
        !statusJson.contains("\"master_key_ok\":true") || !statusJson.contains("\"self_signing_key_ok\":true")
    @JvmStatic fun nativeFormatCrossSigningStatusFallback(statusJson: String): String {
        val masterOk = statusJson.contains("\"master_ok\":true") || statusJson.contains("\"master_key_ok\":true")
        val selfOk = statusJson.contains("\"self_signing_ok\":true") || statusJson.contains("\"self_signing_key_ok\":true")
        val userOk = statusJson.contains("\"user_signing_ok\":true")
        return when {
            masterOk && selfOk && userOk -> "Verified"
            masterOk && selfOk -> "Self-verified"
            masterOk -> "Not verified"
            else -> "Setup needed"
        }
    }

    // --- Native SQLite DB fallbacks ---
    @JvmStatic fun nativeSqliteDbOpenFallback(dbPath: String, key: String): Boolean =
        false // Fallback: no-op, use Realm

    @JvmStatic fun nativeSqliteDbCloseFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbInsertEventFallback(key: String, eventId: String, roomId: String,
        type: String, senderId: String, contentJson: String, originTs: Long,
        ageTs: Long, displayIndex: Int): Boolean = false

    @JvmStatic fun nativeSqliteDbInsertEventRelFallback(key: String, eventId: String, roomId: String,
        type: String, senderId: String, contentJson: String, originTs: Long,
        ageTs: Long, displayIndex: Int, stateKey: String, redacts: String,
        relType: String, relatesToId: String): Boolean = false

    @JvmStatic fun nativeSqliteDbQueryEventsFallback(key: String, roomId: String,
        limit: Int, offset: Int, ascending: Boolean): String = "[]"

    @JvmStatic fun nativeSqliteDbQueryEventFallback(key: String, eventId: String): String = "{}"
    @JvmStatic fun nativeSqliteDbDeleteEventFallback(key: String, eventId: String) {}
    @JvmStatic fun nativeSqliteDbCountEventsFallback(key: String, roomId: String): Int = 0
    @JvmStatic fun nativeSqliteDbMaxDisplayIndexFallback(key: String, roomId: String): Int = 0

    @JvmStatic fun nativeSqliteDbUpsertRoomFallback(key: String, roomId: String,
        displayName: String, avatarUrl: String, topic: String, membership: String,
        notifCount: Int, highlightCount: Int, lastActivityMs: Long,
        isDirect: Boolean, isSpace: Boolean, isFavourite: Boolean, isEncrypted: Boolean): Boolean = false

    @JvmStatic fun nativeSqliteDbQueryRoomsFallback(key: String): String = "[]"
    @JvmStatic fun nativeSqliteDbBeginTransactionFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbCommitTransactionFallback(key: String) {}
    @JvmStatic fun nativeSqliteDbSchemaVersionFallback(key: String): Int = 0

    // --- Event Type / Power Level fallbacks ---
    @JvmStatic fun nativeGetEventTypeDescriptionFallback(eventType: String, msgType: String): String = when(msgType) {
        "m.image" -> "Image"; "m.video" -> "Video"; "m.audio" -> "Audio"; "m.file" -> "File"
        "m.emote" -> "* ${eventType}"; "m.notice" -> "Notice"; else -> "Message"
    }
    @JvmStatic fun nativeGetEventTypeIconFallback(eventType: String, msgType: String): String = when(msgType) {
        "m.image" -> "📷"; "m.video" -> "🎥"; "m.audio" -> "🎵"; "m.file" -> "📎"
        "m.emote" -> "💬"; else -> "💬"
    }
    @JvmStatic fun nativeIsContinuationFallback(curSender: String, prevSender: String, curTs: Long, prevTs: Long): Boolean =
        curSender == prevSender && (curTs - prevTs) < 300_000
    @JvmStatic fun nativeParseRoomPowerLevelsFallback(stateContentJson: String): String =
        """{"users_default":0,"events_default":0,"state_default":50,"ban":50,"kick":50,"redact":50,"invite":50}"""
    @JvmStatic fun nativeHasPowerFallback(plJson: String, userId: String, action: String): Boolean = true

    // --- SSO fallbacks ---
    @JvmStatic fun nativeIsSsoCallbackUrlFallback(url: String): Boolean = url.contains("loginToken")
    @JvmStatic fun nativeExtractSsoProviderFallback(idpId: String): String = when(idpId) {
        "google" -> "Google"; "github" -> "GitHub"; "facebook" -> "Facebook"
        "apple" -> "Apple"; "gitlab" -> "GitLab"; else -> idpId
    }

    // --- Room Tombstone fallback ---
    @JvmStatic fun nativeParseRoomTombstoneContentFallback(stateEventJson: String): String = "{}"

    // --- Network Quality fallback ---
    @JvmStatic fun nativeClassifyNetworkQualityFallback(signalStrength: Int, latencyMs: Double, lossRate: Double): String =
        when {
            lossRate > 10.0 || latencyMs > 500 -> "Poor"
            lossRate > 2.0 || latencyMs > 200 -> "Fair"
            else -> "Good"
        }

    // --- Connection Monitor fallback ---
    @JvmStatic fun nativeFormatDowntimeFallback(downtimeMs: Long): String {
        val s = downtimeMs / 1000
        return when { s < 60 -> "${s}s"; s < 3600 -> "${s/60}m ${s%60}s"; else -> "${s/3600}h ${(s%3600)/60}m" }
    }

    // --- Event Preview fallback ---
    @JvmStatic fun nativeFormatEventPreviewFallback(senderName: String, body: String, eventType: String, msgType: String, showSender: Boolean): String {
        val prefix = if (showSender) "$senderName: " else ""
        return prefix + body.take(120).replace("\n", " ")
    }

    // --- Room Encryption fallbacks ---
    @JvmStatic fun nativeParseEncryptionConfigFallback(stateContentJson: String): String = "{}"
    @JvmStatic fun nativeComputeEncryptionStatusFallback(algorithm: String): String = when {
        algorithm.isEmpty() -> "Not encrypted"
        algorithm.contains("megolm") -> "Encrypted (Megolm)"
        else -> "Encrypted"
    }

    // --- Space Utilities fallback ---
    @JvmStatic fun nativeParseSpaceChildrenFallback(stateEventsJson: String): String = "[]"

    // --- E2EE Decoration fallbacks ---
    @JvmStatic fun nativeGetE2eeIconNameFallback(state: String): String = when {
        state.contains("verified") -> "ic_shield_verified"
        state.contains("warning") -> "ic_shield_warning"
        else -> "ic_shield_black"
    }
    @JvmStatic fun nativeGetE2eeColorFallback(state: String): String = when {
        state.contains("verified") -> "#4CAF50"
        state.contains("warning") -> "#FF9800"
        else -> "#F44336"
    }

    // --- Backup fallbacks ---
    @JvmStatic fun nativeBuildCreateBackupBodyFallback(algorithm: String, authData: String): String =
        """{"algorithm":"$algorithm","auth_data":$authData}"""
    @JvmStatic fun nativeFormatBackupStatsFallback(infoJson: String): String = "Backup stats"
    @JvmStatic fun nativeNeedsBackupAttentionFallback(infoJson: String): Boolean {
        val total = Regex("\"total_keys\":(\\d+)").find(infoJson)?.groupValues?.get(1)?.toIntOrNull() ?: 0
        val backed = Regex("\"backed_up_keys\":(\\d+)").find(infoJson)?.groupValues?.get(1)?.toIntOrNull() ?: 0
        return total > 0 && backed < total
    }

    // --- Room Notif Settings fallback ---
    @JvmStatic fun nativeBuildRoomNotifSettingsBodyFallback(mode: String): String = """{"actions":["$mode"]}"""

    // --- Notifications / Badge fallback ---
    @JvmStatic fun nativeFormatBadgeTextFallback(totalCount: Int): String =
        if (totalCount > 99) "99+" else if (totalCount > 0) "$totalCount" else ""

    // --- Presence fallback ---
    @JvmStatic fun nativeParsePresenceFallback(userId: String, apiResponseJson: String): String =
        """{"user_id":"$userId","presence":"offline","last_active_ago_ms":0}"""

    // --- Matrix Error fallback ---
    @JvmStatic fun nativeGetRetryAfterMsFallback(errorJson: String): Long = 3000L

    // --- OpenID Token fallback ---
    @JvmStatic fun nativeParseOpenIdTokenFallback(json: String): String = "{}"

    // --- Notification Counts fallbacks ---
    @JvmStatic fun nativeFormatCombinedNotificationCountFallback(roomCount: Int, threadCount: Int): String {
        val total = roomCount + threadCount
        return if (total > 99) "99+" else if (total > 0) "$total" else ""
    }
    @JvmStatic fun nativeGetTotalUnreadCountFallback(roomCount: Int, threadCount: Int): Int = roomCount + threadCount

    // --- Presence Indicator fallbacks ---
    @JvmStatic fun nativeGetPresenceIndicatorFallback(presence: String): String = when(presence) {
        "online" -> "🟢"; "unavailable" -> "🟡"; else -> "⚫"
    }
    @JvmStatic fun nativeIsPresenceStaleFallback(lastUpdatedMs: Long): Boolean =
        (System.currentTimeMillis() - lastUpdatedMs) > 300_000
    @JvmStatic fun nativeFormatStatusMessageFallback(message: String, maxLen: Int): String =
        if (message.length <= maxLen) message else message.take(maxLen) + "…"

    // --- Space fallbacks ---
    @JvmStatic fun nativeBuildSpaceChildContentFallback(suggested: Boolean, order: String, autoJoin: Boolean, canonical: Boolean): String =
        """{"suggested":$suggested,"order":"$order","auto_join":$autoJoin}"""
    @JvmStatic fun nativeBuildSpaceParentContentFallback(parentSpaceId: String, canonical: Boolean): String =
        """{"via":[""],"canonical":$canonical}"""

    // --- Widget fallback ---
    @JvmStatic fun nativeParseWidgetStateContentFallback(stateContentJson: String, widgetId: String, roomId: String): String =
        """{"widget_id":"$widgetId","type":"","name":"","url":""}"""

    // --- Key Share fallback ---
    @JvmStatic fun nativeShouldShareKeyFallback(algorithm: String, hasSession: Boolean, sessionVerified: Boolean, userTrusted: Boolean): Boolean =
        hasSession && sessionVerified

    // --- Recovery Key fallbacks ---
    @JvmStatic fun nativeComputeRecoveryKeyFallback(curve25519Key: String): String = curve25519Key
    @JvmStatic fun nativeExtractCurveKeyFromRecoveryKeyFallback(recoveryKey: String): String = recoveryKey.replace(" ", "")

    // --- Membership fallbacks ---
    @JvmStatic fun nativeFormatMembershipFallback(membership: String): String = when(membership) {
        "join" -> "Joined"; "invite" -> "Invited"; "knock" -> "Knocked"; "ban" -> "Banned"; else -> "Left"
    }
    @JvmStatic fun nativeIsActiveMemberFallback(membership: String): Boolean = membership == "join" || membership == "invite"

    // --- Calls fallbacks ---
    @JvmStatic fun nativeBuildCallHangupContentFallback(callId: String, reason: String): String =
        """{"call_id":"$callId","reason":"$reason"}"""
    @JvmStatic fun nativeFormatCallNotificationFallback(callJson: String): String = "Incoming call"

    // --- Content Scanner / ToS fallbacks ---
    @JvmStatic fun nativeIsServerNoticeFallback(eventContentJson: String): Boolean = eventContentJson.contains("\"server_notice\"")
    @JvmStatic fun nativeMustAcceptTosFallback(responseJson: String): Boolean = responseJson.contains("\"m.consent\"")
    @JvmStatic fun nativeBuildTosAcceptBodyFallback(version: String): String = """{"version":"$version"}"""

    // --- Login Utilities fallbacks ---
    @JvmStatic fun nativeIsValidLoginCredentialsFallback(userId: String, password: String): Boolean =
        userId.isNotEmpty() && password.isNotEmpty()
    @JvmStatic fun nativeGenerateDeviceIdFallback(): String =
        "PC" + java.util.UUID.randomUUID().toString().take(8).uppercase()

    // --- Password Validator fallbacks ---
    @JvmStatic fun nativeValidatePasswordFallback(password: String): String {
        val len = password.length
        val hasUpper = password.any { it.isUpperCase() }
        val hasLower = password.any { it.isLowerCase() }
        val hasDigit = password.any { it.isDigit() }
        val valid = len >= 8 && hasUpper && hasLower && hasDigit
        val strength = (len * 5 + (if (hasUpper) 20 else 0) + (if (hasLower) 20 else 0) + (if (hasDigit) 20 else 0)).coerceAtMost(100)
        val label = when { strength >= 80 -> "Strong"; strength >= 60 -> "Good"; strength >= 40 -> "Fair"; else -> "Weak" }
        return """{"valid":$valid,"strength":$strength,"strength_label":"$label","feedback":""}"""
    }
    @JvmStatic fun nativeComputePasswordStrengthFallback(password: String): Int {
        val len = password.length.coerceAtMost(20)
        val classes = listOf(password.any { it.isUpperCase() }, password.any { it.isLowerCase() }, password.any { it.isDigit() }).count { it }
        return (len * 4 + classes * 15).coerceAtMost(100)
    }
    @JvmStatic fun nativeGetStrengthLabelFallback(strength: Int): String = when {
        strength >= 80 -> "Strong"; strength >= 60 -> "Good"; strength >= 40 -> "Fair"; else -> "Weak"
    }
    @JvmStatic fun nativeGeneratePasswordFeedbackFallback(password: String): String = when {
        password.length < 8 -> "Use at least 8 characters"
        !password.any { it.isUpperCase() } -> "Add an uppercase letter"
        !password.any { it.isDigit() } -> "Add a digit"
        else -> ""
    }

    // --- Event Validator fallbacks ---
    @JvmStatic fun nativeIsValidEventIdFallback(eventId: String): Boolean = eventId.startsWith("\$") && eventId.length > 10
    @JvmStatic fun nativeIsValidSenderIdFallback(senderId: String): Boolean = senderId.startsWith("@") && senderId.contains(":")
    @JvmStatic fun nativeIsFileSizeWithinLimitsFallback(fileSize: Long, maxSizeBytes: Long): Boolean = fileSize <= maxSizeBytes

    // --- Invite fallbacks ---
    @JvmStatic fun nativeIsInviteExpiredFallback(invitedAtMs: Long, maxAgeDays: Int): Boolean =
        (System.currentTimeMillis() - invitedAtMs) > maxAgeDays * 86400_000L
    @JvmStatic fun nativeBuildKnockBodyFallback(reason: String): String =
        """{"reason":"$reason"}"""

    // --- Device fallbacks ---
    @JvmStatic fun nativeFormatFingerprintFallback(fingerprint: String): String =
        fingerprint.chunked(4).joinToString(" ")
    @JvmStatic fun nativeIsDeviceInactiveFallback(lastSeenMs: Long): Boolean =
        lastSeenMs > 0 && (System.currentTimeMillis() - lastSeenMs) > 90L * 86400_000

    // --- Version fallback ---
    @JvmStatic fun nativeSatisfiesMinVersionFallback(current: String, minimum: String): Boolean =
        nativeCompareSemverFallback(current, minimum) >= 0

    // --- Federation fallback ---
    @JvmStatic fun nativeParseFederationVersionFallback(json: String): String = """{"name":"","version":""}"""

    // --- Report fallbacks ---
    @JvmStatic fun nativeIsValidReportReasonFallback(reason: String): Boolean = reason.isNotEmpty()
    @JvmStatic fun nativeGetReasonDescriptionFallback(code: String): String = when(code) {
        "m.spam" -> "Spam"; "m.violence" -> "Violence"; "m.hate" -> "Hate speech"
        "m.harassment" -> "Harassment"; "m.suicide" -> "Self-harm"; "m.child" -> "Child exploitation"
        else -> code
    }

    // --- Secret Storage fallbacks ---
    @JvmStatic fun nativeExtractDefaultSecretKeyFallback(accountDataJson: String): String = ""
    @JvmStatic fun nativeHasCrossSigningSecretsFallback(accountDataJson: String): Boolean =
        accountDataJson.contains("\"m.cross_signing")

    // --- Report fallbacks ---
    @JvmStatic fun nativeIsOffensiveFallback(score: Int): Boolean = score < -50
    @JvmStatic fun nativeTruncateReportDescriptionFallback(description: String, maxLen: Int): String =
        if (description.length <= maxLen) description else description.take(maxLen) + "..."

    // --- Content Scanner fallback ---
    @JvmStatic fun nativeIsContentScannerAvailableFallback(serverCapabilitiesJson: String): Boolean = false

    // --- Matrix Error fallbacks ---
    @JvmStatic fun nativeIsPasswordErrorFallback(errorCode: String): Boolean = errorCode == "M_WEAK_PASSWORD"
    @JvmStatic fun nativeGetAllErrorCodesFallback(): String = """["M_UNKNOWN","M_FORBIDDEN","M_BAD_JSON","M_NOT_JSON","M_NOT_FOUND","M_LIMIT_EXCEEDED","M_UNKNOWN_TOKEN","M_MISSING_TOKEN","M_WEAK_PASSWORD","M_INVALID_USERNAME","M_USER_IN_USE","M_EXCLUSIVE","M_THREEPID_IN_USE","M_THREEPID_NOT_FOUND","M_SERVER_NOT_TRUSTED","M_CONSENT_NOT_GIVEN","M_UNSUPPORTED_ROOM_VERSION","M_INCOMPATIBLE_ROOM_VERSION","M_CANNOT_LEAVE_SERVER_NOTICE_ROOM","M_RESOURCE_LIMIT_EXCEEDED"]"""

    // --- Call Content fallbacks ---
    @JvmStatic fun nativeBuildCallInviteContentFallback(callId: String, isVideo: Boolean, sdpOffer: String, lifetimeSec: Int): String =
        """{"call_id":"$callId","offer":{"type":"offer","sdp":"$sdpOffer"},"version":1,"lifetime":$lifetimeSec}"""
    @JvmStatic fun nativeBuildCallAnswerContentFallback(callId: String, sdpAnswer: String): String =
        """{"call_id":"$callId","answer":{"type":"answer","sdp":"$sdpAnswer"},"version":1}"""
    @JvmStatic fun nativeGetCallStateFallback(eventContentJson: String): String = "unknown"

    // --- Megolm fallbacks ---
    @JvmStatic fun nativeMegolmAddSessionFallback(roomId: String, senderKey: String, sessionId: String, sessionKeyBase64: String): Boolean = false
    @JvmStatic fun nativeMegolmDecryptFallback(roomId: String, senderKey: String, sessionId: String, ciphertext: String): String = ""
    @JvmStatic fun nativeMegolmSessionCountFallback(): Int = 0
    @JvmStatic fun nativeMegolmClearRoomFallback(roomId: String) {}

    // --- Olm Account fallbacks ---
    @JvmStatic fun nativeOlmCreateAccountFallback(userId: String, deviceId: String): Boolean = false
    @JvmStatic fun nativeOlmGetIdentityKeysFallback(): String = "{}"
    @JvmStatic fun nativeOlmGenerateOneTimeKeysFallback(count: Int): String = "{}"
    @JvmStatic fun nativeOlmSignMessageFallback(message: String): String = ""
    @JvmStatic fun nativeOlmCreateInboundSessionFallback(theirIdentityKey: String, preKeyMessage: String): String = ""
    @JvmStatic fun nativeOlmDecryptMessageFallback(senderKey: String, sessionId: String, ciphertext: String): String = ""
    @JvmStatic fun nativeOlmPickleAccountFallback(): String = ""
    @JvmStatic fun nativeOlmUnpickleAccountFallback(pickled: String, userId: String, deviceId: String): Boolean = false

    // --- Device Verification fallbacks ---
    @JvmStatic fun nativeVerifyDeviceSignatureFallback(deviceKeysJson: String, userId: String, deviceId: String, signKeyB64: String, signatureB64: String): Boolean = false
    @JvmStatic fun nativeComputeDeviceFingerprintFallback(identityKeyBase64: String): String =
        identityKeyBase64.take(20).chunked(4).joinToString(" ") { it.uppercase() }

    // --- SAS fallbacks ---
    @JvmStatic fun nativeSasCreateFallback(): String = ""
    @JvmStatic fun nativeSasSetTheirKeyFallback(theirPubkey: String): Boolean = false
    @JvmStatic fun nativeSasGetEmojisFallback(): String = "[]"
    @JvmStatic fun nativeSasCalculateMacFallback(input: String, info: String): String = ""
    @JvmStatic fun nativeSasVerifyMacFallback(theirMac: String, input: String, info: String): Boolean = false
    @JvmStatic fun nativeSasDestroyFallback() {}

    // --- URL Preview fallbacks ---
    @JvmStatic fun nativeIsPreviewableUrlFallback(url: String): Boolean = url.startsWith("http")
    @JvmStatic fun nativeExtractUrlsFallback(text: String): String {
        val regex = Regex("https?://[^\\s]+")
        return regex.findAll(text).joinToString(",", "[", "]") { "\"${it.value}\"" }
    }

    // --- Device Manager fallback ---
    @JvmStatic fun nativeFormatDeviceLastSeenFallback(lastSeenMs: Long): String {
        if (lastSeenMs <= 0) return "Never"
        val diff = (System.currentTimeMillis() - lastSeenMs) / 1000
        return when {
            diff < 60 -> "Active now"
            diff < 3600 -> "${diff / 60}m ago"
            diff < 86400 -> "${diff / 3600}h ago"
            else -> "${diff / 86400}d ago"
        }
    }

    // --- Permalink fallback ---
    @JvmStatic fun nativeIsSameRoomPermalinkFallback(url1: String, url2: String): Boolean {
        val id1 = Regex("#/([!@#][^/?]+)").find(url1)?.groupValues?.get(1) ?: ""
        val id2 = Regex("#/([!@#][^/?]+)").find(url2)?.groupValues?.get(1) ?: ""
        return id1 == id2 && id1.isNotEmpty()
    }

    // --- Display Name fallbacks ---
    @JvmStatic fun nativeGetBestDisplayNameFallback(displayName: String, userId: String): String =
        displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
    @JvmStatic fun nativeFormatMemberNameFallback(displayName: String, userId: String, powerLevel: Int, showBadge: Boolean): String {
        val name = displayName.ifEmpty { userId.removePrefix("@").substringBefore(":") }
        return if (showBadge && powerLevel >= 50) "$name ⭐" else name
    }

    // --- Identity fallbacks ---
    @JvmStatic fun nativeIsEmailFallback(input: String): Boolean = input.contains("@") && input.contains(".")
    @JvmStatic fun nativeIsMsisdnFallback(input: String): Boolean = input.startsWith("+") && input.drop(1).all { it.isDigit() }
    @JvmStatic fun nativeExtractAliasLocalpartFallback(alias: String): String = alias.removePrefix("#").substringBefore(":")

    // --- Link Preview fallback ---
    @JvmStatic fun nativeIsImageUrlFallback(url: String): Boolean =
        url.endsWith(".jpg") || url.endsWith(".png") || url.endsWith(".gif") || url.endsWith(".webp")

    // --- Permalink Parser fallbacks ---
    @JvmStatic fun nativeExtractRoomIdFromPermalinkFallback(url: String): String =
        Regex("[!#][^:/?#]+").find(url)?.value ?: ""
    @JvmStatic fun nativeExtractEventIdFromPermalinkFallback(url: String): String =
        Regex("\\\$[^:/?#]+").find(url)?.value ?: ""
    @JvmStatic fun nativeExtractUserIdFromPermalinkFallback(url: String): String =
        Regex("@[^:/?#]+").find(url)?.value ?: ""

    // --- URL Preview fallbacks ---
    @JvmStatic fun nativeStripHtmlTagsFallback(html: String): String = html.replace(Regex("<[^>]*>"), " ").replace(Regex("\\s+"), " ").trim()
    @JvmStatic fun nativeTruncateDescriptionFallback(text: String, maxLen: Int): String =
        if (text.length <= maxLen) text else text.take(maxLen).substringBeforeLast(" ") + "…"

    // --- Device Type fallback ---
    @JvmStatic fun nativeClassifyDeviceTypeFallback(userAgent: String, clientName: String): String = when {
        userAgent.contains("Mobile") || userAgent.contains("Android") -> "Mobile"
        userAgent.contains("Tablet") -> "Tablet"
        else -> "Desktop"
    }

    // --- Version Comparison fallback ---
    @JvmStatic fun nativeCompareSemverFallback(a: String, b: String): Int {
        val ap = a.split(".").map { it.toIntOrNull() ?: 0 }
        val bp = b.split(".").map { it.toIntOrNull() ?: 0 }
        for (i in 0 until maxOf(ap.size, bp.size)) {
            val av = ap.getOrElse(i) { 0 }; val bv = bp.getOrElse(i) { 0 }
            if (av != bv) return av.compareTo(bv)
        }
        return 0
    }

    // --- Knock / Server / Fingerprint fallbacks ---
    @JvmStatic fun nativeFormatKnockReasonFallback(reason: String): String = reason.ifEmpty { "No reason provided" }
    @JvmStatic fun nativeIsServerCompatibleFallback(serverVersion: String, minRequired: String): Boolean {
        val r = nativeCompareSemverFallback(serverVersion, minRequired)
        return r >= 0
    }
    @JvmStatic fun nativeExtractDeviceFingerprintFallback(deviceId: String, keysJson: String): String = deviceId.take(10)

    // --- Event Distance fallback ---
    @JvmStatic fun nativeEventDistanceFallback(indexA: Int, indexB: Int): Int = kotlin.math.abs(indexA - indexB)

    // --- Widget Validation fallbacks ---
    @JvmStatic fun nativeIsEtherpadWidgetFallback(type: String): Boolean = type == "m.etherpad" || type == "etherpad"
    @JvmStatic fun nativeIsValidWidgetUrlFallback(url: String): Boolean = url.startsWith("https://")

    // --- Encryption Defaults fallbacks ---
    @JvmStatic fun nativeGetDefaultEncryptionAlgorithmFallback(): String = "m.megolm.v1.aes-sha2"
    @JvmStatic fun nativeRequiresDeviceVerificationFallback(algorithm: String): Boolean =
        !algorithm.isEmpty() && algorithm != "m.olm.v1.curve25519-aes-sha2"

    // --- Timestamp fallback ---
    @JvmStatic fun nativeIsReasonableTimestampFallback(originServerTs: String, maxFutureMs: Long): Boolean {
        val ts = originServerTs.toLongOrNull() ?: return true
        return (System.currentTimeMillis() - ts) < maxFutureMs
    }

}
