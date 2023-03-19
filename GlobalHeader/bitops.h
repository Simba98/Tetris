
#ifndef __BITOPS_H__
#define __BITOPS_H__
//bit����ģ��
#define BITMASK(b) (1 << ((b) % CHAR_BIT)) 
#define BITSLOT(b) ((b) / CHAR_BIT) 
#define BITSET(a, b) ((a)[BITSLOT(b)] |= BITMASK(b)) 
#define BITCLEAR(a, b) ((a)[BITSLOT(b)] &= ~BITMASK(b)) 
#define BITTEST(a, b) ((a)[BITSLOT(b)] & BITMASK(b)) 
#define BITNSLOTS(nb) ((nb + CHAR_BIT - 1)/CHAR_BIT)
//char���Ͳ���
#define CHARHBIT(a,b) ((a)&(0xFF<<(8-(b))))
#define CHARHBITL(a,b) ((a)>>(8-(b)))  
#define CHARLBIT(a,b) ((a)&(0xFF>>(8-(b))))	//��bλ��1-8λ�Կɡ�
#define CHARATOBBIT(a,b,c) ((a)&((0xFF>>(7+(b)-(c)))<<(b)))//��0λ����7λ
#define CHARATOBBITL(a,b,c) (((a)&((0xFF>>(7+(b)-(c)))<<(b)))>>(b))//��0λ����7λ
#define CHARH4BIT(a) ((a)&0xF0)
#define CHARH4BITL(a) ((a)>>4)  
#define CHARL4BIT(a) ((a)&0x0F)

#endif