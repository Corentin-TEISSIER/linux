// SPDX-License-Identifier: GPL-2.0
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/iio/iio.h>
#include <linux/iio/sysfs.h>
#include <linux/module.h>

struct bh1745_data {
	struct mutex mutex;
};

static int bh1745_read_raw(struct iio_dev *indio_dev,
			   struct iio_chan_spec const *chan,
			   int *val, int *val2, long mask)
{
	pr_info("Called read\n");

	return 0;
}

static int bh1745_write_raw(struct iio_dev *indio_dev,
			    struct iio_chan_spec const *chan,
			    int val, int val2, long mask)
{
	pr_info("Called write\n");

	return 0;
}

static const struct iio_info bh1745_info = {
	.read_raw = bh1745_read_raw,
	.write_raw = bh1745_write_raw,
};

static const struct iio_chan_spec bh1745_channels[] = {
	{
		.type = IIO_LIGHT,
		.info_mask_separate = BIT(IIO_CHAN_INFO_RAW)
	}
};

static int bh1750_probe(struct i2c_client *client)
{
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
	struct bh1745_data *data;
	struct iio_dev *indio_dev;
	int ret;

	pr_info("Hello Coco!\n");

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C |
				I2C_FUNC_SMBUS_WRITE_BYTE))
		return -EOPNOTSUPP;

	indio_dev = devm_iio_device_alloc(&client->dev, sizeof(*data));
	if (!indio_dev)
		return -ENOMEM;

	data = iio_priv(indio_dev);
	i2c_set_clientdata(client, indio_dev);
	data->client = client;

	mutex_init(&data->mutex);
	indio_dev->info = &bh1745_info;
	indio_dev->name = id->name;
	indio_dev->channels = bh1750_channels;
	indio_dev->num_channels = ARRAY_SIZE(bh1750_channels);
	indio_dev->modes = INDIO_DIRECT_MODE;

	ret = iio_device_register(indio_dev);
	if (ret)
		return ret;

	pr_info("Probe done succesfully\n");

	return 0;
}

static void bh1750_remove(struct i2c_client *client)
{
	struct iio_dev *indio_dev = i2c_get_clientdata(client);
	struct bh1750_data *data = iio_priv(indio_dev);

	iio_device_unregister(indio_dev);
}

static const struct i2c_device_id bh1750_id[] = {
	{ "bh1745", 0 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, bh1750_id);

static struct i2c_driver bh1750_driver = {
	.driver = {
		.name = "bh1750",
	},
	.probe_new = bh1750_probe,
	.remove = bh1750_remove,
	.id_table = bh1750_id,

};
module_i2c_driver(bh1750_driver);
