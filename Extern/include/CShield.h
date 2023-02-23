#ifdef CShield_EXPORTS
#define CShield_API __declspec(dllexport)
#else
#define CShield_API __declspec(dllimport)
#endif

typedef struct _CShieldData {
	DWORD dwThreadId;
	HANDLE hThread;
	DWORD dwStartAddress;
	DWORD dwUser32Low;
	DWORD dwUser32Hi;
	DWORD dwMemType;
	DWORD dwMemAllocationBase;
} CShieldData;

extern CShieldData myCShieldData;

CShield_API CShieldData InitializeCShield();
CShield_API bool CheckAttackspeed(float attackSpeed, float range, bool twoHanded, bool twoHandedHorse);
CShield_API bool CheckMovespeed(float moveSpeed);
CShield_API bool CheckValues();
CShield_API std::string GenKey(bool isGamePhase, DWORD num);
extern CShield_API int clientCode;
extern CShield_API char charName[24];