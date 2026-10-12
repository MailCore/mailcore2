package com.libmailcore.androidexample

import android.os.Bundle
import android.util.Log
import androidx.appcompat.app.AppCompatActivity
import com.libmailcore.androidexample.databinding.ActivityMainBinding
import com.libmailcore.ConnectionType
import com.libmailcore.IMAPSession
import com.libmailcore.IMAPMessagesRequestKind
import com.libmailcore.IndexSet
import com.libmailcore.MailException
import com.libmailcore.OperationCallback
import com.libmailcore.Range

/**
 * Minimal mailcore2 sample: connect to an IMAP account over TLS, list the folders,
 * then fetch the headers of the most recent messages in INBOX — all through the
 * com.libmailcore API provided by mailcore2-android-4.aar.
 *
 * mailcore runs each operation on a background queue and delivers the
 * OperationCallback on the main (UI) thread, so the UI updates below are safe.
 */
class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.connectButton.setOnClickListener { startSync() }

        // Allows headless launch: `am start -n .../.MainActivity --ez autostart true`
        if (intent?.getBooleanExtra("autostart", false) == true) startSync()
    }

    private fun log(line: String) = runOnUiThread {
        binding.outputView.append(line + "\n")
        Log.i(TAG, line)
    }

    private fun startSync() {
        binding.outputView.text = ""
        binding.connectButton.isEnabled = false

        val session = IMAPSession()
        session.setHostname(binding.hostField.text.toString().trim())
        session.setPort(binding.portField.text.toString().trim().toIntOrNull() ?: 993)
        session.setUsername(binding.userField.text.toString().trim())
        session.setPassword(binding.passField.text.toString())
        session.setConnectionType(ConnectionType.ConnectionTypeTLS)

        log("Connecting to ${binding.hostField.text}…")

        val foldersOp = session.fetchAllFoldersOperation()
        foldersOp.start(object : OperationCallback {
            override fun succeeded() {
                val folders = foldersOp.folders()
                log("✓ Connected. Folders (${folders.size}):")
                folders.forEach { log("   • ${it.path()}") }
                fetchInbox(session)
            }
            override fun failed(e: MailException) = finishWith("✗ Folder list failed: ${e.message}")
        })
    }

    private fun fetchInbox(session: IMAPSession) {
        val infoOp = session.folderInfoOperation("INBOX")
        infoOp.start(object : OperationCallback {
            override fun succeeded() {
                val count = infoOp.info().messageCount()
                log("\nINBOX: $count messages")
                if (count <= 0) { finishWith("\n✓ Done"); return }

                val from = maxOf(1, count - 9)
                // Range spans location..location+length (inclusive), so length is the delta,
                // not the element count: a single message is Range(n, 0).
                val length = (count - from).toLong()
                val kind = IMAPMessagesRequestKind.IMAPMessagesRequestKindHeaders or
                        IMAPMessagesRequestKind.IMAPMessagesRequestKindFlags or
                        IMAPMessagesRequestKind.IMAPMessagesRequestKindUid
                val indexes = IndexSet.indexSetWithRange(Range(from.toLong(), length))

                val fetchOp = session.fetchMessagesByNumberOperation("INBOX", kind, indexes)
                fetchOp.start(object : OperationCallback {
                    override fun succeeded() {
                        val messages = fetchOp.messages()
                        log("\nLast ${messages.size} messages:")
                        messages.reversed().forEach { m ->
                            val header = m.header()
                            val subject = header?.subject() ?: "(no subject)"
                            val sender = header?.from()?.let { it.displayName() ?: it.mailbox() } ?: "?"
                            log("   [${m.uid()}] $sender — $subject")
                        }
                        finishWith("\n✓ Done")
                    }
                    override fun failed(e: MailException) =
                        finishWith("✗ Message fetch failed (code=${e.errorCode()}): ${e.message}")
                })
            }
            override fun failed(e: MailException) = finishWith("✗ INBOX info failed: ${e.message}")
        })
    }

    private fun finishWith(message: String) {
        log(message)
        runOnUiThread { binding.connectButton.isEnabled = true }
    }

    companion object {
        private const val TAG = "MailCoreExample"
    }
}
