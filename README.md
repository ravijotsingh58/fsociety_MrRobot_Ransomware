# fsociety_MrRobot_Ransomware
This is Fsociety Ransomware coded by Darlene in MrRobot (S1E2,3)

## On Movie

![alt text](https://camo.githubusercontent.com/2346efca54bee1dc4a867c939caab5c556970c042f60cd4a4d9d46660868b40b/687474703a2f2f7777772e77656c69766573656375726974792e636f6d2f77702d636f6e74656e742f75706c6f6164732f323031362f30372f6d72726f626f7431312d31303234783539302e6a7067)

## Required lib:
- Crypto++
(steps to compile and use Crypto++ :- https://github.com/weidai11/cryptopp/blob/master/Readme.txt)
- CURL

## Ransomware step:
- Initial
- File listing
- File parser
- Encrypt files source with AES Library
- Change filename with filename+.locked
- Change desktop background to fsociety (if OS is windows)
- Send to jester C&C (Number file...)
- Ask reward & setup jester pages

## Notes:
- For security reason the ransomware crypt only .txt but you can change on c++ file
- windows not contains dirent.h, so i have to get it separately from some website. I have provided that file also for windows.

## please remember this is just a PoC the ransomware is not stable is just for fun!

## Advanced
In future i'll add multiple step (Decrypt one file for free, FAQ ...).

## DISCLAIMER
This is Jester ransomware like, in Mr Robot movie (coded by Darlene S1E2,3), Please beware warning, after start you can't recover the files (In movie, the public key is not shared). Please don't use this malware on personal computer. This project is just a wink of mr robot series.
