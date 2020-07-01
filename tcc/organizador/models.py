from django.db import models
from django.utils import timezone
# Create your models here.

class Item(models.Model):
    TAM = [
        ('P','Pequeno'),
        ('M','Medio'),
        ('G','Grande'),
    ]
    Tipo =[
        ('S','Superior'),
        ('I','Inferior'),
    ]
    descricao = models.CharField(max_length=200)
    tamanho = models.CharField(max_length=200,choices=TAM,default='P')
    tipo = models.CharField(max_length=200,choices=Tipo,default='S')
    cor = models.CharField(max_length=200)
    disponibilidade = models.BooleanField()
    foto = models.ImageField(upload_to='foto')
    rfid = models.CharField(max_length=200,unique=True)

    def __str__(self):
        return self.descricao

class Fluxo(models.Model):
    TP = [
        ('E','Entrou'),
        ('S','Saiu'),
    ]
    data = models.DateTimeField(default=timezone.now())
    item = models.ForeignKey(Item, on_delete=models.CASCADE)
    direcao = models.CharField(max_length=200,choices=TP,default='S')
    realizado = models.BooleanField(default=False)

    def __str__(self):
        return str(self.data) +"-" + self.item.descricao
    
class Favorito(models.Model):
    Superior = models.ForeignKey(Item, on_delete=models.CASCADE,related_name='Superior',blank=True)
    Inferior = models.ForeignKey(Item, on_delete=models.CASCADE,related_name='Inferior',blank=True)