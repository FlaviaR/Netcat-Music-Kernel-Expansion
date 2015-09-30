#define pr_fmt(fmt) "[" KBUILD_MODNAME "]: " fmt

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>

#include "netcat.h"


struct netcat {
	char	*msg;
	bool	first_time;
	int	current_track;
};

struct _netcmd {
	bool next;
	bool prev;
	int track;
	bool beg;
	bool rand; 
	bool stop;
};

static struct _netcmd netcmd = { false, false, -1, false, false, false };

/// The maximum length of the message from the device.
#define BUF_LEN 512
/// The message the device will return
static char msg[BUF_LEN];
static char *msg_ptr;

#define DEVICE_NAME "netcat"	/* Dev name as it appears in /proc/devices   */

extern struct netcat_track netcat_cpi_trk1;
extern struct netcat_track netcat_cpi_trk2;
extern struct netcat_track netcat_cpi_trk3;
extern struct netcat_track netcat_cpi_trk4;
extern struct netcat_track netcat_cpi_trk5;
extern struct netcat_track netcat_cpi_trk6;
extern struct netcat_track netcat_cpi_trk7;
extern struct netcat_track netcat_cpi_trk8;
extern struct netcat_track netcat_cpi_trk9;

static struct netcat_track *tracks[] = {
					&netcat_cpi_trk1,
					&netcat_cpi_trk2,
					&netcat_cpi_trk3,
					&netcat_cpi_trk4,
					&netcat_cpi_trk5,
					&netcat_cpi_trk6,
					&netcat_cpi_trk7,
					&netcat_cpi_trk8,
					&netcat_cpi_trk9};

static int device_open(struct inode *inode, struct file *file)
{
	struct netcat *netcat;

	netcat = kzalloc(sizeof(*netcat), GFP_KERNEL);
	if (!netcat)
		return -ENOMEM;

	netcat->first_time = true;
	netcat->current_track = 0;
	netcat->msg = tracks[0]->data;	/* track 1 */
	msg_ptr = msg; // for writing
	file->private_data = netcat;
	return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
	struct netcat *netcat = file->private_data;

	kfree(netcat);
	return 0;
}

int m_w = 3;    /* must not be zero, nor 0x464fffff */
int m_z = 6;    /* must not be zero, nor 0x9068ffff */

//simulated random number generation 
uint get_random(void) {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    return (m_z << 16) + m_w;  /* 32-bit result */
}

static ssize_t device_read(struct file *file,
			   char *buffer,
			   size_t length,
			   loff_t *offset)
{
	struct netcat *netcat = file->private_data;
	int current_track = netcat->current_track;

	int bytes_read = 0;

	if (netcat->first_time) {
		pr_info("Now playing track %d - %s\n", current_track + 1, tracks[current_track]->name);
		netcat->first_time = false;
	}

	if ( netcat->msg - tracks[current_track]->data >= tracks[current_track]->len ) {
		/* End of Track.  Skip to next track, or finish if it's the last track */
		if ( netcmd.prev ) {
			current_track = current_track - 1;
			if ( current_track < 0 ) current_track = ARRAY_SIZE(tracks) - 1;
			netcmd.prev = false;
		} 
		
		else if ( netcmd.next ) {
			current_track = (current_track + 1) % ARRAY_SIZE(tracks);
   	 	netcmd.next = false;
		}
		
		else if ( netcmd.track > -1 ) {
			current_track = netcmd.track % ARRAY_SIZE(tracks);
			netcmd.track = -1;

		}
		
		else if ( netcmd.beg ) {
			//no need to change current track as this wont make much of a difference
			//we just have to be sure that the else condition wont be reached
			netcmd.beg = false;
		}		
		
		else if ( netcmd.rand ) {
			current_track = get_random() % ARRAY_SIZE(tracks);
			netcmd.rand = false;
		}
		
		else if ( netcmd.stop ){
			return 0;		
		}
		
		else { //none of the above 
			current_track = (current_track + 1) % ARRAY_SIZE(tracks); //play next song 
		}
		
		pr_info("Now playing track %d - %s\n", current_track + 1, tracks[current_track]->name );
		netcat->msg = tracks[current_track]->data;
		netcat->current_track = current_track;
	}

	while (length && (netcat->msg - tracks[current_track]->data) < tracks[current_track]->len) {
		if ( netcmd.next || netcmd.prev || netcmd.track > -1 || netcmd.beg || netcmd.rand) {
	        netcat->msg = tracks[current_track]->data + tracks[current_track]->len; //force the loop to overflow		
		} 
	
		else if ( netcmd.stop ) {
			return bytes_read;
		}		
		
		else {
			put_user(*(netcat->msg++), buffer++);
			length--;
			bytes_read++;
			break;
		}
	}

	return bytes_read;
}

static ssize_t device_write(struct file *filp, const char *buffer, size_t len, loff_t *off)
{

	// use "echo p > /dev/netcat" from another terminal to communicate (write to the device)
	// or in this case, ./cmd.sh p as root 
	if (len >= BUF_LEN) {
		// printk("Buffer overflow!\n");
		len = BUF_LEN-1;
		// return -EINVAL;
	}

	// Copy a block of data from user space.
	// mess:    Destination address, in kernel space.
	// buffer:  Source address, in user space.
	// n:       Number of bytes to copy.
	if ( copy_from_user(msg, buffer, len) )
		return  -EFAULT;

	msg[len] = '\0';
	switch (msg[0]) {
		case 'n':
			netcmd.next = true;
			break;
			
		case 'p':
			netcmd.prev = true;
			break;
			
		case 'b':
			netcmd.beg = true;
			break;
			
		case '0' ... '9':
			netcmd.track = msg[0] - '0';
			break;
			
		case 'r':
			netcmd.rand = true;
			break;
			
		case 's':
			netcmd.stop = true;
			break;
	}
	printk("%s", msg);
	return len;
}

static const struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

static struct miscdevice netcat_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = DEVICE_NAME,
	.fops = &fops,
	.mode = S_IRUGO | S_IWUGO,	// read and write
};

static int netcat_init(void)
{
	int ret;

	ret = misc_register(&netcat_dev);
	if (ret) {
		pr_err("misc device register failed\n");
		return ret;
	}
	pr_info("netcat - Cycles Per Instruction - Kernel Module Edition - 2014\n");
	pr_info("use reload.sh and cmd.sh to play\n");

	return 0;
}

static void netcat_exit(void)
{
	misc_deregister(&netcat_dev);
}

module_init(netcat_init);
module_exit(netcat_exit);
