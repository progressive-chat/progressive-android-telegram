package chat.progressive.app.features.home

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import androidx.core.view.isVisible
import androidx.fragment.app.Fragment
import androidx.lifecycle.lifecycleScope
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import chat.progressive.app.native.ProgressiveNative
import im.vector.app.R
import im.vector.app.databinding.FragmentTelegramChatListBinding
import im.vector.app.databinding.ItemTelegramChatBinding
import kotlinx.coroutines.flow.collectLatest
import kotlinx.coroutines.launch
import java.text.SimpleDateFormat
import java.util.Date
import java.util.Locale

class TelegramChatListFragment : Fragment() {

    private var _binding: FragmentTelegramChatListBinding? = null
    private val views get() = _binding!!

    override fun onCreateView(inflater: LayoutInflater, container: ViewGroup?, savedInstanceState: Bundle?): View {
        _binding = FragmentTelegramChatListBinding.inflate(inflater, container, false)
        return views.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        super.onViewCreated(view, savedInstanceState)

        views.telegramChatListRecycler.layoutManager = LinearLayoutManager(requireContext())

        lifecycleScope.launch {
            TelegramChatRepository.isConnected.collectLatest { connected ->
                if (!connected) {
                    views.telegramChatListStatus.isVisible = true
                    views.telegramChatListStatus.text = getString(R.string.tg_chat_list_connecting)
                }
            }
        }

        lifecycleScope.launch {
            TelegramChatRepository.chats.collectLatest { chatList ->
                views.telegramChatListStatus.isVisible = false

                if (chatList.isEmpty()) {
                    views.telegramChatListStatus.isVisible = true
                    views.telegramChatListStatus.text = getString(R.string.tg_chat_list_empty)
                } else {
                    views.telegramChatListRecycler.adapter = ChatAdapter(chatList) { chat ->
                        onChatClicked(chat)
                    }
                }
            }
        }

        TelegramChatRepository.loadChats()
    }

    private fun onChatClicked(chat: TelegramChatSummary) {
        TelegramChatRepository.openChat(chat.id)
        val intent = TelegramChatActivity.newIntent(requireContext(), chat.id, chat.title)
        startActivity(intent)
    }

    override fun onDestroyView() {
        super.onDestroyView()
        _binding = null
    }

    private class ChatAdapter(
        private var chats: List<TelegramChatSummary>,
        private val onClick: (TelegramChatSummary) -> Unit
    ) : RecyclerView.Adapter<ChatHolder>() {

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): ChatHolder {
            val binding = ItemTelegramChatBinding.inflate(LayoutInflater.from(parent.context), parent, false)
            return ChatHolder(binding, onClick)
        }

        override fun onBindViewHolder(holder: ChatHolder, position: Int) {
            holder.bind(chats[position])
        }

        override fun getItemCount() = chats.size
    }

    private class ChatHolder(
        private val binding: ItemTelegramChatBinding,
        private val onClick: (TelegramChatSummary) -> Unit
    ) : RecyclerView.ViewHolder(binding.root) {

        fun bind(chat: TelegramChatSummary) {
            binding.itemTelegramTitle.text = chat.title
            binding.itemTelegramLastMsg.text = chat.lastMessage.ifEmpty { " " }
            if (chat.lastMessageTime > 0) {
                binding.itemTelegramTime.text = formatTime(chat.lastMessageTime)
            } else {
                binding.itemTelegramTime.text = ""
            }
            binding.itemTelegramUnread.isVisible = chat.unreadCount > 0
            if (chat.unreadCount > 0) {
                binding.itemTelegramUnread.text = if (chat.unreadCount > 99) "99+" else chat.unreadCount.toString()
            }
            binding.itemTelegramProtocolDot.backgroundTintList =
                if (chat.isChannel) android.content.res.ColorStateList.valueOf(0xFF2AABEE.toInt())
                else android.content.res.ColorStateList.valueOf(0xFF34A853.toInt())
            binding.root.setOnClickListener { onClick(chat) }
        }

        private fun formatTime(timestamp: Long): String {
            val date = Date(timestamp * 1000L)
            val now = System.currentTimeMillis()
            return if (now - date.time < 24 * 3600 * 1000) {
                SimpleDateFormat("HH:mm", Locale.getDefault()).format(date)
            } else {
                SimpleDateFormat("dd/MM", Locale.getDefault()).format(date)
            }
        }
    }
}
