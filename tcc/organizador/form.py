from django import forms

from .models import Item

class addItem(forms.ModelForm):
	class Meta:
		model = Item
		exclude = ['posicao']

class filtro(forms.Form):
	TAM = [
		('',''),
        ('P','Pequeno'),
        ('M','Medio'),
        ('G','Grande'),
    ]
	COR = [
    	('',''),
		('PR','Preto'),
		('AZ','Azul'),
		('BR','Branco'),
		('VM','Vermelho'),
		('AM','Amarelo'),
		('VE','Verde'),
	]
	tamanho = forms.ChoiceField(choices=TAM,required=False,)
	cor = forms.ChoiceField(choices=COR,required=False)