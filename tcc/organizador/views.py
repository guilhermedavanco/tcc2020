from django.shortcuts import render,redirect
from .models import Item,Fluxo,Escolha
from .form import addItem
from django.http import HttpResponse
from random import randrange

# Create your views here.

def selecao(request):
	atual=Escolha.objects.all()
	return render(request,'organizador/Selecao.html',{'atual':atual})

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

def sugestoes(request):
	sup=Item.objects.filter(tipo="S")
	inf=Item.objects.filter(tipo="I")
	n_sup=len(sup)
	n_inf=len(inf)
	conjunto=[]
	for x in range(4):
		conjunto.append((sup[randrange(n_sup)],inf[randrange(n_inf)]))	
	print(conjunto)
	return render(request,'organizador/Sugestoes.html',{'conj':conjunto})

def escolhe(request):
	peca = request.POST.get('peca')
	tipo = request.POST.get('tipo')
	teste = Escolha.objects.all()
	inst=Item.objects.filter(id=peca)[0]
	teste = Escolha.objects.all()
	if teste:
		if tipo == 'S':
			Escolha.objects.all().update(superior=inst)
		elif tipo == 'I':
			Escolha.objects.all().update(inferior=inst)
	else:
		if tipo == 'S':
			Escolha.objects.create(superior=inst)
		elif tipo == 'I':
			Escolha.objects.create(inferior=inst)
	return HttpResponse('ok!')

def retirar(request):
	superior = request.POST.get('peca1')
	inferior = request.POST.get('peca2')
	print(superior + inferior)
	inst=Item.objects.filter(id=superior)[0]
	inst2=Item.objects.filter(id=inferior)[0]
	Fluxo.objects.create(item=inst)
	Fluxo.objects.create(item=inst2)
	return HttpResponse('ok!')
