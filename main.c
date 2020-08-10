/* include/sharp/shdiag_smd.h

#define SHDIAG_SMD_DEVFILE "/dev/smd_read"

#define SHDIAG_IOC_MAGIC 's'
#define SHDIAG_IOCTL_SET_QXDMFLG     _IOW  (SHDIAG_IOC_MAGIC,  1, unsigned char)

int main() {
printf("SHDIAG_IOCTL_SET_QXDMFLG = %x\n",SHDIAG_IOCTL_SET_QXDMFLG);
}
//SHDIAG_IOCTL_SET_QXDMFLG = 40017301

/* tewilove sharp diag enable ioctl
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

int main(int argc, char *argv[]) {
int fd, rc;
char enabled = 1;

fd = open("/dev/smd_read", O_RDONLY);
if (fd < 0)
return -1;
rc = ioctl(fd, 0x40017301, &enabled);
close(fd);

return rc;
}

/* drivers/sharp/shdiag/smd_read.c

static int smd_ioctl_set_qxdmflg(unsigned long arg)
{
	int ret = 0;
	unsigned char qxdm_work;
	sharp_smem_common_type *sharp_smem;
	
	if(copy_from_user(&qxdm_work, (unsigned short __user *)arg, sizeof(unsigned char)) != 0)
	{
		printk("[SH]smd_ioctl_set_qxdmflg: copy_to_user FAILE\n");
		ret = -EFAULT;
	}
	else
	{
		sharp_smem = sh_smem_get_common_address();//get the SMEM start address pointer
		sharp_smem->shusb_qxdm_ena_flag = qxdm_work;//set the QXDM bit flag pointer
	}
	
	return ret;
}

static long smd_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) //ioctl("/dev/smd_read", SHDIAG_IOCTL_SET_QXDMFLG, 1);
{
	int ret;
	
	switch(cmd) {
	case SHDIAG_IOCTL_SET_QXDMFLG: //parse the arg
		ret = smd_ioctl_set_qxdmflg(arg);
		break;
	case SHDIAG_IOCTL_SET_PROADJ:
		ret = smd_ioctl_set_proadj(arg);
		break;
	case SHDIAG_IOCTL_GET_HW_REVISION:
		ret = smd_ioctl_get_hw_revision(arg);
		break;
	case SHDIAG_IOCTL_SET_HAPTICSCAL:
		ret = smd_ioctl_set_hapticscal(arg);
		break;
	case SHDIAG_IOCTL_SET_MSMFB_OVERLAY_ID:
		ret = smd_ioctl_set_msmfb_overlay_id(arg);
		break;
	case SHDIAG_IOCTL_GET_MSMFB_OVERLAY_ID:
		ret = smd_ioctl_get_msmfb_overlay_id(arg);
		break;
	default:
		printk("[SH]smd_ioctl: cmd FAILE\n");
		ret = -EFAULT;
		break;
	}

	return ret;
}
