# QT_PJLink
PJLink for Qt with authentication .
Tested with Sanyo PLC Projector

/* Authentication test on ubuntu
 * 1. Download packet sender
 * 2. Connect to PJLink projector
 * 3. send packet(Keep alive connection)
 * 4. PJlink will reply with PJLINK <class> <random number> <CR>
 * 5. Create MD5 hash of wth the random number and password.
 *    https://www.md5hashgenerator.com/
 *    MD5( (random number) + your password).
 *    Hashed string is constructed as follows <random_numer><your_password>
 * 6. Send hashed string and command in same packet to projector.
 * 7. Projetor response random response, if PJLINK ERRA is returend. Hashed string was wrong.
 * Example, Password 1323, command %1POWR 1
 * Connect
 * ResponsePJLINK 1 00003a9e
 * MD5 hash(00003a9e1323)
   Send: 3f89cff497594c407e98ab892561b87a%1POWR 1\r
   Response %1POWR=ERR3
   Responded error because projector was already on
*/
