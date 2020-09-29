"""tcc URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.urls import path
from . import views

urlpatterns = [
    path('', views.selecao, name='selecao'),
    path('superior/', views.SuperiorListView.as_view(), name='superior'),
    path('inferior/', views.InferiorListView.as_view(), name='inferior'),
    path('add/', views.add, name='add'),
    path('favoritos/', views.favoritos, name='favoritos'),
    path('sugestoes/', views.sugestoes, name='sugestoes'),
    path('superior/ajax/escolhe/', views.escolhe, name='ajax/escolhe'),
    path('inferior/ajax/escolhe/', views.escolhe, name='ajax/escolhe'),
    path('sugestoes/ajax/escolhe/', views.escolhe, name='ajax/escolhe'),
    path('favoritos/ajax/escolhe/', views.escolhe, name='ajax/escolhe'),
    path('ajax/retirar/', views.retirar, name='ajax/retirar'),
    path('ajax/favoritar/', views.favoritar, name='ajax/favoritar'),
    path('teste/', views.SuperiorListView.as_view(), name='teste')
]
