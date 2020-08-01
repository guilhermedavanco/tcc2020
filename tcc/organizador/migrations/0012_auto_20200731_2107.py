# Generated by Django 2.2.7 on 2020-08-01 00:07

import datetime
from django.db import migrations, models
import django.db.models.deletion
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('organizador', '0011_auto_20200723_1854'),
    ]

    operations = [
        migrations.AlterField(
            model_name='escolha',
            name='inferior',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='Inf', to='organizador.Item'),
        ),
        migrations.AlterField(
            model_name='escolha',
            name='superior',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, related_name='Sup', to='organizador.Item'),
        ),
        migrations.AlterField(
            model_name='fluxo',
            name='data',
            field=models.DateTimeField(default=datetime.datetime(2020, 8, 1, 0, 7, 45, 203575, tzinfo=utc)),
        ),
    ]
