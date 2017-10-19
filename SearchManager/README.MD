# RetroSpy Search Manager Server
This is an emulator of GPSP Server (gpsp.gamespy.com)

Fetures
===

- Full SQLite v3 Support

- EMail Validation (\vaild\) and Nickname Search (\nicks\) are fully supported
This server successfully authenticate an existing user on GameSpy Arcade (Email Valid,Nick Search and Multi Nicks)

Missing Features
===

- MySQL/MariaDB Support

- All the rest

- \search\ have 2 parametra that i don't know : skip\(int) and \bsrdone\more\(int). The server will work without them unless i discover their function

Notes
===

- There could be a way to force GameSpy Arcade to open the in-app registration page (i see the "New User" button is present on gptest)

- Database is WIP, i try to add as many colums as the SDK requires. It also miss a user creation.

- You could easy manage your SQLite database using "DB Browser for SQLite"

- There's not a "real" wrapper to SQLite, most of the functions are written using manual SQLite code. For allowing more people to write the code without knowing sqlite, i will try to move this functions later...

- It could be great to move the "check" of the parametra (like email, namespaceid) into it's own header to allow simplier code read
