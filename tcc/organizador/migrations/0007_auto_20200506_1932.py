# Generated by Django 2.2.7 on 2020-05-06 22:32

import datetime
from django.db import migrations, models
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('organizador', '0006_auto_20200506_1903'),
    ]

    operations = [
        migrations.AddField(
            model_name='fluxo',
            name='realizado',
            field=models.BooleanField(default=False),
        ),
        migrations.AlterField(
            model_name='fluxo',
            name='data',
            field=models.DateTimeField(default=datetime.datetime(2020, 5, 6, 22, 32, 36, 834838, tzinfo=utc)),
        ),
    ]
