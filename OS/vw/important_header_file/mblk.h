/* mblk (message buffer) primitives
 * Copyright 1999 YMC, BDCOM 
 *
 */
#ifndef __MBLK_H
#define __MBLK_H
#include <gsblk.h>

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef NULLBUF
#define	NULLBUF	(mblk_t *)0
#endif
#ifndef NULLBUFP
#define	NULLBUFP (mblk_t **)0
#endif
#define	NULLMBLK (mblk_t *)0
#define	NULLMBLKP (mblk_t **)0
#define NULLCHAR (char *)0
#define	NULLBUFD (dblk_t *)0
#define	HEADER_SIZE      256

#define	PULLCHAR(bpp)\
 ((bpp) != NULL && (*bpp) != NULLBUF && ((*bpp)->b_wptr - (*bpp)->b_rptr) > 1 ? \
 ((*bpp)->flen-- ,(unsigned char)*(*bpp)->b_rptr++): pullchar(bpp))

#define get16(buf)	(*(USHORT*)(buf))
#define get32(buf)	(*(ULONG*)(buf))
#define put16(buf,x)	(*(USHORT*)(buf)=x)
#define put32(buf,x)	(*(ULONG*)(buf)=x)

#define COPY_MB_USER_DATA(nbp,bp)	{(nbp)->user_data=(bp)->user_data;SET_FLEN(nbp,GET_FLEN(bp));}
#define GET_MB_USER_DATA(bp)		((bp)->user_data)
#define SET_MB_USER_DATA(bp,ud) ((bp)->user_data=ud)

#define ADD_FLEN(bp,size)		((bp)->flen +=size)
#define SET_FLEN(bp,len)		((bp)->flen =len)
#define GET_FLEN(bp)			(mb_len(bp)) /* ((bp)->flen) later*/

void hex_dump(unsigned char *buf, long buflen);

/*internal functions*/
char * do_mb_addhdr(mblk_t **bph, long size, unsigned long LR);
mblk_t * do_mb_allocb(long size, unsigned long LR);
void  do_mb_append(mblk_t **bph,mblk_t *bp, unsigned long LR);
long do_mb_copyp(mblk_t *bp, char *buf, long cnt, unsigned long LR);
unsigned long do_mb_copyb(mblk_t *bp, char *buf, unsigned long offset, unsigned long cnt, unsigned long LR);
long do_mb_dump(unsigned char * comment, mblk_t *bp, long offset, long cnt, unsigned long LR);
mblk_t* do_mb_freeb(mblk_t *bp, unsigned long LR);
mblk_t *do_mb_freep(mblk_t *bp, unsigned long LR);
char * do_mb_insert(mblk_t **bpp, long cnt, long offset, unsigned long LR);
long do_mb_len(mblk_t *mbuf, unsigned long LR);
long do_mb_num(mblk_t *mbuf, unsigned long LR);
long do_mb_count(mblk_t *mbuf, unsigned long LR);
long do_mb_pullup_offset(mblk_t **bph,char *buf,long cnt,long offset, unsigned long LR);
long do_mb_pullup_offset0(mblk_t **bph, char *buf, long cnt, unsigned long LR);
mblk_t * do_mb_pushdown(mblk_t *bp, char *buf, long size, unsigned long LR);
long do_mb_dqdata(mblk_t *bp, unsigned char *buf, long cnt, unsigned long LR);
mblk_t * do_mb_qdata(unsigned char *data, long cnt, unsigned long LR);
mblk_t *do_mb_split(mblk_t *bp, long size, unsigned long LR);
void  do_mb_trim(mblk_t **bpp, long length, unsigned long LR);
void do_mb_adj(mblk_t *mp, int req_len, unsigned long LR);
long do_pull32(mblk_t **bpp, unsigned long LR);
long do_pull16(mblk_t **bpp, unsigned long LR);
int do_pullchar(mblk_t **bpp, unsigned long LR);


/* Insert specified amount of contiguous new space at the beginning of an
 * mblk chain. If enough space is available in the first mblk, no new space
 * is allocated. Otherwise a mblk of the appropriate size is allocated and
 * tacked on the front of the chain.
 * Here,we only leave space for you to add,you should read mblk->b_rptr to
 * write it.
 * Notice: Don't write more than the size!
 */
char * mb_addhdr(mblk_t **bph,long size);

/* Allocate mblk, return NULL if memory is unavailable */
mblk_t * mb_allocb(long size);
/* Append mblk to end of mblk chain */
void  mb_append(mblk_t **bph,mblk_t *bp);

/* Copy first 'cnt' bytes of packet into a buf */
long mb_copyp(mblk_t *bp,char *buf,long cnt);

/*************************************************
*  Function: mb_copyb()  Copy 'cnt' bytes of packet into a buf from specified offset
*  paremeter:
*			mblk_t        	*bp      : the dump mblk	
*			char 			*buf		: buf space			
*           unsinged long 	offset   : offset 
*			unsinged long   cnt      :  lenth to copy
*	return:
		bytes copied
***************************************************/
unsigned long mb_copyb(mblk_t *bp, char *buf, unsigned long offset, unsigned long cnt);

/*************************************************
*  Function: mb_dump()  dump packet from mblk (printf)
*  paremeter:
*			unsigned char *comment : comment for the dump packet
*									 Can be string or NULL
*			mblk_t        *bp      : the dump mblk	
*           long          offset   : offset 
*			long          cnt      : length of dump
*  sun   12/02/99      
***************************************************/
long mb_dump(unsigned char * comment,mblk_t *bp,long offset,long cnt);

/* Decrement the reference pointer in an mblk. If it goes to zero,
 * free all resources associated with mblk.
 * Return pointer to next mblk in packet b_cont chain
 */
mblk_t* mb_freeb(mblk_t *bp);

/* 
Free packet (a chain of mblks). Return pointer to next packet on b_next chain,
*/
mblk_t *  mb_freep(mblk_t *bp);

/*************************************************
* Insert specified amount of contiguous new space at the middle of an
 * mblk chain. 
 * Here,we only leave space for you to add,you should read mblk->b_rptr to
 * write it.The function return the empty buf pointer.
 * Notice: Don't write more than the size!
* if offset great than whole space, return NULL  
插入指定的空间大小到mblk的offset处，如果offset为0，则相当于在头部插入了指定大小的空间。
如果offset大于整个mblk空间，则返回NULL,返回新生成的mblk chain的指针。
*  Function: mb_insert()  
*  paremeter:
*			mblk_t  **bpp      : dst mblk ptr ,maybe modified
*           long 	cnt   :  insert space cnt 
*			long   	offset :  insert offset
*	return:
		new inserted space ptr
***************************************************/
char * mb_insert(mblk_t **bpp,long cnt,long offset);

/* Count up the total number of bytes in a packet */
long mb_len(mblk_t *mblk);

/* Count up the number of packets in a b_cont chain */
long mb_num(mblk_t *mblk);

/* Count up the number of packets in a b_next chain */
long mb_count(mblk_t *mbuf);


/* Copy and delete "cnt" bytes from "offset" of packet. Return number of
 * bytes actually pulled off
 */
long mb_pullup_offset(mblk_t **bph,char *buf,long cnt,long offset);
long mb_pullup_offset0(mblk_t **bph,char *buf,long cnt);
#define mb_pullup(bph, buf, cnt, offset) \
	((offset)?mb_pullup_offset(bph, buf, cnt, offset): mb_pullup_offset0(bph, buf, cnt))
#define do_mb_pullup(bph, buf, cnt, offset, LR) \
	((offset)?do_mb_pullup_offset(bph, buf, cnt, offset, LR): do_mb_pullup_offset0(bph, buf, cnt, LR))



/*************************************************
* Insert specified amount of contiguous new space at the beginning of an
 * mblk chain. If enough space is available in the first mblk, no new space
 * is allocated. Otherwise a mblk of the appropriate size is allocated and
 * tacked on the front of the chain.
 * This operation is the logical inverse of pullup(), hence the name.
扩展指定大小的空间到mblk的头部，并COPY相应BUF内容到扩展的头部空间中，如果BUF为NULL,则
只扩展头部空间，不做COPY动作,bp是欲扩展空间的mblk指针，返回值为新的mblk指针，如果头部空间足够，则直接
扩展，如果不足，则分配新的mblk头并进行扩展
*  Function: mb_pushdown()  
*  paremeter:
*			mblk_t  *bp      : src mblk to be insert head
*           char  	*buf   : if buf is null, only insert new space, or copy buf to new space
*			long   	size :  insert offset
*	return:
		new mblk ptr ,if failed ,return NULL
***************************************************/
mblk_t * mb_pushdown(mblk_t *bp,char *buf,long size);

/*************************************************
 COPY mblk里指定cnt的内容到buf中，如果buf为NULL,则立即返回，如果cnt大于mblk里的内容，则只COPY mblk
 里的内容， COPY完成后，会释放bp
*  Function: mb_dqdata()  
*  paremeter:
*			mblk_t  *bp      : mblk ptr
*           char  	*buf   : user buf, if NULL, return 0 immedially
*			long   	cnt :  bytes cnt to be copied
*	return:
		实际COPY的字节数
***************************************************/
long mb_dqdata(mblk_t *bp,unsigned char *buf,long cnt);

/*************************************************
 分配一个mblk,将data指向的数据COPY进去，长度为cnt 
 里的内容
*  Function: mb_qdata()  
*  paremeter:
*           unsigned char  	*data   : data space
*			long   	cnt :  bytes cnt to be copied
*	return:
		已经COPY好的mblk指针
***************************************************/
mblk_t * mb_qdata(unsigned char *data,long cnt);

/* Resemble the mblk b_next link to the link with b_cont 
 * Return : Pointer to the new mblk*/
mblk_t * mb_assem(mblk_t *bp);

/* Split the mblk chain to fix size b_next chain
 * Return: pointer to the new mblk chain
 *
 *Notice : No copy function! The new mblk chain will share datab with
 *	   the old mblk.
 */
mblk_t * mb_split(mblk_t *bp,long size);

/* Trim mblk to specified length by lopping off end */
/*************************************************
 将指定mblk的内容裁剪到指定的length长度，如果length为0，则释放整个mblk,
 如果length大于mblk的内容，则只设置mblk的flen值，但并不做其它改动(这样处理有问题？？？？)
 指定length之后的mblk内容被释放
*  Function: mb_trim()  
*  paremeter:
*           unsigned char  	*bpp   : mblk ptr to be trimed
*			long   	length :  bytes cnt to be copied
***************************************************/
void  mb_trim(mblk_t **bpp,long length);


/* adjust mblk content to rip off length from mblk */
/*************************************************
 将指定mblk的内容裁剪出指定的长度，假如req_len>0,则从头部裁剪掉指定长度的内容
	假如req_len<0,则从尾部裁剪掉指定长度的内容
*  Function: mb_trim()  
*  paremeter:
*           unsigned char  	*mp   : mblk ptr to be adjust
*			int   	req_len :  adjust lenth
***************************************************/
void mb_adj(mblk_t *mp, int req_len);

/* Pull a 32-bit integer in host order from buffer in network byte order.
 * On error, return 0. Note that this is indistinguishable from a normal
 * return.
 */
long pull32(mblk_t **bpp);

/* Pull a 16-bit integer in host order from buffer in network byte order.
 * Return -1 on error
 */
long pull16(mblk_t **bpp);

/* Pull single character from mblk */
int pullchar(mblk_t **bpp);

/*record mblk delivery course*/
void mb_record(mblk_t *p);

/*记录:在某个位置曾经持有该mblk,用于追踪mblk的传递轨迹*/
#define register_one_mblk(m) mb_record(m)
#define unregister_one_mblk(m) mb_record(m)
#define register_mblks(m) mb_record(m)
#define unregister_mblks(m) mb_record(m)

/*判断dblk是否繁忙,占用80%以上就认为繁忙*/
int dblk_is_busy(int size);

#ifdef __cplusplus
    }
#endif

#endif
