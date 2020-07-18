from django.shortcuts import render,redirect
from .models import Item,Fluxo
from .form import addItem

# Create your views here.

def selecao(request):
	return render(request,'organizador/Selecao.html')
def superior(request):
	pecas=Item.objects.filter(tipo="S")
	print(pecas)
	return render(request,'organizador/Superior.html',{'pecas':pecas})
def inferior(request):
	pecas=Item.objects.filter(tipo="I")
	print(pecas)

	return render(request,'organizador/Inferior.html',{'pecas':pecas})


def add(request):
	if request.method == "POST":
		print('entrou!')
		form = addItem(request.POST, request.FILES)
		print(form)
		if form.is_valid():
			form.save()
			return redirect('/')
		else:
			print('nao entrou')
	else:
		form = addItem()
	return render(request,'organizador/add.html',{'form':form})


def favoritos(request):
	return render(request,'organizador/Favoritos.html')