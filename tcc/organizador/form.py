from django import forms

from .models import Item

class addItem(forms.ModelForm):
	class Meta:
		model = Item
		exclude = ['posicao']	