from django.shortcuts import render
from .models import Item,Fluxo

# Create your views here.

def selecao(request):
	return render(request,'organizador/Selecao.html')
def superior(request):
	return render(request,'organizador/Selecao.html')
def inferior(request):
	return render(request,'organizador/Selecao.html')
def add(request):
	return render(request,'organizador/Selecao.html')
def favoritos(request):
	return render(request,'organizador/Selecao.html')