/*
 * =====================================================================================
 *
 *       Filename:  dj_yaffs2.c
 *       Description:   
 *       Author: djstava , djstava@gmail.com
 *       WebSite: http://macernow.com/
 *
 * =====================================================================================
 */

#include "dj_yaffs2.h"
#include "dj_trace.h"
#include "dj_base.h"

/*
**  Macros
*/
#define MAX_PAGE_SIZE	4096
#define MAX_OOB_SIZE	218

/* Buffer array used for writing data*/
DJ_UINT8 writebuf[MAX_PAGE_SIZE];
DJ_UINT8 oobbuf[MAX_OOB_SIZE];
DJ_UINT8 oobreadbuf[MAX_OOB_SIZE];


/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_yaffs2_erase_mtd
 *      Description: erase a yaffs2 mtd
 *	  Params: 
 *      	1 DJ_UINT8 *mtdDevice,mtd device name,/dev/mtd? for example
 *      Return: 
 *		true: DJ_SUCCESS
 *		false: DJ_FAILURE
 *
 *======================================================================================
 */
 #if 0
DJ_INT32 dj_yaffs2_erase_mtd(DJ_UINT8 *mtdDevice)
{
	libmtd_t mtd_desc;
	struct mtd_dev_info mtd;
	DJ_INT32 fd;
	DJ_UINT64 start;
	DJ_UINT32 eb, eb_start, eb_cnt;
	DJ_UINT64 offset = 0;
	DJ_INT32 ret;

	//start = simple_strtoull(0, &error);
	//eb_cnt = simple_strtoul(0, &error);
	start = 0;
	eb_cnt = 0;

	/*
	 * Locate MTD and prepare for erasure
	 */
	mtd_desc = libmtd_open();
	if (mtd_desc == NULL)
	{
		dj_printf("[DJ_YAFFS2] Can't initialize libmtd.\n");
		return DJ_FAILURE;
	}

	if ((fd = open(mtdDevice, O_RDWR)) < 0)
	{
		dj_printf("[DJ_YAFFS2] Open %s error.\n", mtdDevice);
		return DJ_FAILURE;
	}

	if (mtd_get_dev_info(mtd_desc, mtdDevice, &mtd) < 0)
	{
		dj_printf("[DJ_YAFFS2] mtd_get_dev_info failed.\n");
		return DJ_FAILURE;
	}
	eb_start = start / mtd.eb_size;

	/*
	 * Now do the actual erasing of the MTD device
	 */
	if (eb_cnt == 0)
		eb_cnt = (mtd.size / mtd.eb_size) - eb_start;

	for (eb = eb_start; eb < eb_start + eb_cnt; eb++) 
	{
		offset = (DJ_UINT64)eb * mtd.eb_size;

		ret = mtd_is_bad(&mtd, fd, eb);
		if(ret > 0)
		{
			dj_printf("[DJ_YAFFS2] Skipping bad block at %llu.\n",offset);
			continue;
		}

		if (mtd_erase(mtd_desc, &mtd, fd, eb) != 0) 
		{
			dj_printf("[DJ_YAFFS2]:%s: MTD Erase failure", mtdDevice);
			continue;
		}

	}
	libmtd_close(mtd_desc);

	return DJ_SUCCESS;
}


/*=========================FUNCTION===================================================== 
 *
 *      Name: dj_yaffs2_write_mtd
 *      Description: write image to a yaffs2 mtd
 *	  Params: 
 *      	1 DJ_UINT64 addr,mtd start address
 *		2 DJ_UINT64 imgSize,image size
 *		3 DJ_UINT8 *img,image buf
 *      Return:  DJ_VOID
 *
 *======================================================================================
 */
DJ_INT32 dj_yaffs2_write_mtd(DJ_UINT64 addr ,DJ_UINT64 imgSize,DJ_UINT8 *img)
{
	DJ_INT32 mtdoffset = 0;
	DJ_INT32 fd,pagelen, baderaseblock, blockstart = -1;
	struct mtd_info_user meminfo;
	struct mtd_write_req oob;
	loff_t offs;
	DJ_INT32 ret, readlen;
	DJ_INT8 mtd_device[256]={0};
	DJ_UINT32 mtd_num=0;
	struct nand_oobinfo old_oobinfo;
	DJ_UINT64 imglen = 0;
	DJ_UINT32 sndMsgFlag = 0;
	DJ_INT32 rtn;

	memset(oobbuf, 0xff, sizeof(oobbuf));

	rtn = lj_iploader_get_mtd_number(addr,&mtd_num);
	if(rtn != D_SUCCESS)
	{
		printf("lj_iploader_write_yaffs2_mtd Get 0x%llx mtd number error.\n",addr);
		return DJ_FAILURE;
	}
	sprintf(mtd_device,"/dev/mtd%d",mtd_num);
	dj_printf("Writing--- %s Total:%llu.\n",mtd_device,imgSize);

	//lj_iploader_get_mtd_size(mtd_device,&mtdSize); 
	//printf("mtdSize=0x%llx\n",mtdSize);
	//DG_AL_NV_Flash_Erase(addr, mtdSize);
	ret = lj_iploader_erase_mtd(mtd_device);
	if(-1 == ret)
	{
		dj_printf("Erase %s failed.\n",mtd_device);
	}
	else
	{
		dj_printf("Erase %s success.\n",mtd_device);
	}
		
	/* Open the device */
	if ((fd = open(mtd_device, O_RDWR)) == -1) {
		dj_printf("open flash %s error\n",mtd_device);
		return DJ_FAILURE;
	}

	/* Fill in MTD device capability structure */
	if (ioctl(fd, MEMGETINFO, &meminfo) != 0) {
		dj_printf("MEMGETINFO %s error\n",mtd_device);
		close(fd);
		return DJ_FAILURE;
	}

	/* Make sure device page sizes are valid */
	if (!(meminfo.oobsize == 16 && meminfo.writesize == 512) &&
		!(meminfo.oobsize == 8 && meminfo.writesize == 256) &&
		!(meminfo.oobsize == 64 && meminfo.writesize == 2048)&&
		!(meminfo.oobsize == 218 && meminfo.writesize == 4096)) {
		dj_printf("Unknown flash (not normal NAND) %s error\n",mtd_device);
		close(fd);
		return DJ_FAILURE;
	}

	/* Read the current oob info */
	if (ioctl (fd, MEMGETOOBSEL, &old_oobinfo) != 0) {
		dj_printf("MEMGETOOBSEL %s error.\n",mtd_device);
		close (fd);
		return DJ_FAILURE;
	}

	oob.len = meminfo.writesize;
	oob.ooblen = meminfo.oobsize;
	oob.mode = 0;
	oob.usr_data = (unsigned long long)writebuf;
	oob.usr_oob = (unsigned long long)oobbuf;

	/* get image length*/
	imglen = (unsigned long long)imgSize;
	//lseek (ifd, 0, SEEK_SET);

	pagelen = meminfo.writesize + meminfo.oobsize ;

	/*Check, if file is pagealigned*/
	if ( (imglen % pagelen) != 0) {
		dj_printf("Input file %s is not page aligned.\n",img);
		close (fd);
		return DJ_FAILURE;
	}

	// Check, if length fits into device
	if ( ((imglen / pagelen) * meminfo.writesize) > (meminfo.size - mtdoffset)) 
	{
		//printf ("Image %d bytes, NAND page %d bytes, OOB area %u bytes, device size %u bytes\n",
				//imglen, pagelen, meminfo.writesize, meminfo.size);
		dj_printf("Input file does not fit into device.\n");
		close (fd);
		return DJ_FAILURE;
	}

	/* Get data from input and write to the device */
	while (imglen && (mtdoffset < meminfo.size)) 
	{
		// new eraseblock , check for bad block(s)
		// Stay in the loop to be sure if the mtdoffset changes because
		// of a bad block, that the next block that will be written to
		// is also checked. Thus avoiding errors if the block(s) after the
		// skipped block(s) is also bad (number of blocks depending on
		// the blockalign
		while (blockstart != (mtdoffset & (~meminfo.erasesize + 1))) {

			blockstart = mtdoffset & (~meminfo.erasesize + 1);
			offs = blockstart;
				baderaseblock = 0;
			dj_printf ("Writing data to block %x\n", blockstart);

				/* Check all the blocks in an erase block for bad blocks */
			do {
				if ((ret = ioctl(fd, MEMGETBADBLOCK, &offs)) < 0) {
					dj_printf("ioctl(MEMGETBADBLOCK)\n");
					close (fd);
					return DJ_FAILURE;
				}
				if (ret == 1) {
					baderaseblock = 1;
					dj_printf ("Bad block at %x, from %x will be skipped\n", (int) offs, blockstart);
				}

				if (baderaseblock) {
					mtdoffset = blockstart + meminfo.erasesize;
				}
					offs +=  meminfo.erasesize	;
			} while ( offs < blockstart + meminfo.erasesize );

		}

		readlen = meminfo.writesize;
		memset(writebuf,0x0,sizeof(writebuf));
		/* Read Page Data from input file */
		memcpy(writebuf,img,readlen);

		img += readlen;
		//	 printf("writesize:0x%x,oobsize:0x%x\n",meminfo.writesize,meminfo.oobsize);

		/* Read OOB data from input file, exit on failure */
		memcpy(oobreadbuf,img,meminfo.oobsize);
		img += meminfo.oobsize;

		{
			memset(oobbuf,0xff,sizeof(oobbuf));
			memcpy(oobbuf, oobreadbuf , meminfo.oobsize);	
		}

		oob.start = mtdoffset;

		/* Write OOB data first, as ecc will be placed in there*/
		if (ioctl(fd, MEMWRITE, &oob) != 0) 
		{
			printf("ioctl(MEMWRITEOOB)\n");
			close (fd);

			return DJ_FAILURE;
		}
		
		imglen -= readlen;
		imglen -= meminfo.oobsize;
		mtdoffset += meminfo.writesize;
		sndMsgFlag += (readlen + meminfo.oobsize);

	}

	close(fd);
	if (imglen > 0) {
		dj_printf("Data did not fit into device, due to bad blocks.\n");
		return DJ_FAILURE;
	}
	dj_printf("lj_iploader_write_yaffs2_mtd success.\n");

	return DJ_SUCCESS;
}
#endif

