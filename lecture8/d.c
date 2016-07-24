#include <linux/gpio.h>
#include <linux/irq.h>
#include <linux/interrupt.h>

#define GPIO_NUM 67
#define GPIO_NAME "Interrupt Line"
#define IRQ_NAME "GPIO IRQ"
static int gpio_irq_num = 0;

irqreturn_t irq_handler (int irq, void *notused)
{
printk("OK!");

return IRQ_HANDLED;
}


static int __init d_init(void)
{
	gpio_request (GPIO_NUM, GPIO_NAME);
	gpio_irq_num = gpio_to_irq (GPIO_NUM);	

	request_irq (gpio_irq_num, irq_handler, 0, IRQ_NAME, NULL);
	irq_set_irq_type(gpio_irq_num, IRQ_TYPE_EDGE_RISING);
	
	return 0;
}

static void __exit d_exit(void)
{
	free_irq(gpio_irq_num, NULL);
	gpio_free (GPIO_NUM);
}

module_init (d_init);
module_exit (d_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Example character driver");
MODULE_AUTHOR("student");




