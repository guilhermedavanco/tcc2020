from django.contrib import admin
from .models import Item,Fluxo,Escolha,Favorito
# Register your models here.

admin.site.register(Item)
admin.site.register(Fluxo)
admin.site.register(Escolha)
admin.site.register(Favorito)