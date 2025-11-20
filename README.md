<b><h1>AutoSuspend</h1></b>

AutoSuspend is a lightweight C-based Windows utility that automatically monitors active processes and suspends idle ones to conserve system resources. Ideal for users seeking performance and thermal efficiency, especially on constrained hardware.



<h2>🔧 Features</h2>

- ✅ Automatically suspends/resumes processes based on CPU activity
- ✅ Detects processes with visible windows
- ✅ Excludes critical system and user-defined processes
- ✅ Dynamically add program to excluded if hung and can't be resumed
- ✅ Logs all actions to `SuspendLog.txt`
- ✅ Displays a startup notification when launched
- ✅ Mirrors log output to terminal for real-time feedback
- ✅ Network Monitoring to prevent failed downloading files and apps 

<h2>🚀 Build Instructions</h2>

Using Code::Blocks

1. Install Code::Blocks with MinGW
2. Create a new Console Application (C language)
3. Add all `.c` and `.h` files to the project
4. In Project → Build Options → Linker Settings, add:
   ```
   psapi
   ```
5. Build and run the project

Using GCC (Command Line)

bash
gcc main.c process_utils.c window_utils.c cpu_utils.c log_utils.c notify_utils.c -o AutoSuspend.exe -lpsapi -Wall

<h2>🖥️ Usage</h2>

1. Launch `AutoSuspend.exe`
2. A startup notification will confirm the program is running
3. The program monitors processes every 10 seconds
4. Idle processes are suspended; active ones are resumed
5. All actions are logged to `service_log.txt` and printed to the terminal


<h2>⚙️ Customization</h2>

- Modify the exclusion list in `process_utils.c`
- Adjust monitoring interval via `Sleep(10000)` in `main.c`
- Dynamically add hung processess to excluded if can't be resumed


<h2>🧠 Technical Notes</h2>

- Uses Windows APIs: `CreateToolhelp32Snapshot`, `SuspendThread`, `GetProcessTimes`, etc.
- Written entirely in C (not C++)



<h2>📜 License</h2>

This project is free to use and modify for personal, educational, or research purposes. For commercial distribution, please include attribution.

AutoSuspend は軽量な C 言語ベースの Windows ユーティリティで、アクティブなプロセスを自動的に監視し、アイドル状態のものを一時停止してシステムリソースを節約します。特に制約のあるハードウェア環境で、パフォーマンスや発熱効率を求めるユーザーに最適です。

<h2>🔧 機能 </h2>
✅ CPU アクティビティに基づいてプロセスを自動的に一時停止／再開

✅ 可視ウィンドウを持つプロセスを検出

✅ 重要なシステムプロセスやユーザー定義プロセスを除外

✅ 再開できずハングした場合は動的に除外リストへ追加

✅ すべての動作を SuspendLog.txt に記録

✅ 起動時に通知を表示

✅ ログ出力をターミナルにミラーリングしてリアルタイム確認可能

✅ ネットワーク監視機能によりダウンロード中のファイルやアプリの失敗を防止

<h2>🚀 ビルド手順 </h2>
Code::Blocks を使用する場合
MinGW 付きの Code::Blocks をインストール

新しいコンソールアプリケーション（C 言語）を作成

すべての .c と .h ファイルをプロジェクトに追加

Project → Build Options → Linker Settings で以下を追加:

Code
psapi
プロジェクトをビルドして実行

GCC（コマンドライン）を使用する場合
bash
gcc main.c process_utils.c window_utils.c cpu_utils.c log_utils.c notify_utils.c -o AutoSuspend.exe -lpsapi -Wall

<h2>🖥️ 使い方</h2>
AutoSuspend.exe を起動

起動通知が表示され、プログラムが稼働していることを確認

プロセスは 10 秒ごとに監視されます

アイドル状態のプロセスは一時停止され、アクティブなものは再開されます

すべての動作は service_log.txt に記録され、ターミナルにも表示されます

<h2>⚙️ カスタマイズ</h2>
除外リストは process_utils.c 内で編集可能

監視間隔は main.c の Sleep(10000) を変更して調整可能

再開できないハングしたプロセスは動的に除外リストへ追加

<h2>🧠 技術ノート</h2>
使用する Windows API: CreateToolhelp32Snapshot, SuspendThread, GetProcessTimes など

すべて C 言語で記述（C++ ではない）


<h2>📜 ライセンス</h2>
本プロジェクトは個人利用、教育目的、研究目的で自由に使用・改変可能です。商用配布の場合は、必ず著作権表示を含めてください。
