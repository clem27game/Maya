
# 🤖 Guide Discord.my - Créer des Bots Discord avec Maya

## 🚀 Introduction

Discord.my est un module révolutionnaire de Maya v6.0 qui permet de créer des bots Discord complets directement en langage Maya! 

## 📋 Installation et Lancement

### Lancer un Bot Discord
```bash
./main Discord.my-allume mon_bot.my
```

### Structure de Base
```my
# Configuration obligatoire
Discord.my.connect('TOKEN_DU_BOT', 'PREFIX')

# Le bot reste actif jusqu'à arrêt manuel!
```

## 🔗 Fonctions Principales

### 1. Connexion du Bot
```my
Discord.my.connect('YOUR_BOT_TOKEN', '!')
```
- **token**: Token de votre bot Discord
- **prefix**: Préfixe des commandes (ex: !, $, ?)

### 2. Statut et Activité
```my
Discord.my.status('en ligne')  # en ligne, inactif, ne pas déranger
Discord.my.activity('playing', 'avec Maya!')  # playing, listening, watching
```

### 3. Intents (Permissions)
```my
Discord.my.intent('message_content')
Discord.my.intent('guild_members')
Discord.my.intent('guild_presences')
```

## 💬 Création de Commandes

### Commandes Simples
```my
Discord.my.command('salut', 'Bonjour! Comment allez-vous?')
Discord.my.command('aide', 'Voici la liste des commandes disponibles...')
```

### Commandes avec Embeds
```my
Discord.my.embed('info', 'Titre de l\'Embed', 'Description détaillée', '#ff0000', 'Footer', 'https://image-url.com/image.png')
```

**Paramètres de l'embed:**
1. **trigger**: Déclencheur de la commande
2. **title**: Titre de l'embed
3. **description**: Description/contenu principal
4. **color**: Couleur en hexadécimal (optionnel)
5. **footer**: Texte en bas de l'embed (optionnel)
6. **image**: URL d'une image (optionnel)

## 🔒 Gestion des Permissions

### Définir des Permissions
```my
Discord.my.command('ban', 'Utilisateur banni!')
Discord.my.permission('ban', 'BAN_MEMBERS')
Discord.my.error('ban', 'Vous n\'avez pas la permission de bannir!')
```

### Permissions Disponibles
- `KICK_MEMBERS`
- `BAN_MEMBERS`
- `MANAGE_MESSAGES`
- `MANAGE_CHANNELS`
- `MODERATE_MEMBERS`
- `ADMINISTRATOR`

## ⚡ Commandes de Modération

### Kick et Ban
```my
Discord.my.kick('user_id', 'Raison du kick')
Discord.my.ban('user_id', 'Raison du ban')
```

### Gestion des Channels
```my
Discord.my.channel('create_text', 'nouveau-salon')
Discord.my.channel('create_voice', 'Salon Vocal')
Discord.my.channel('clone', 'salon-existant')
Discord.my.channel('delete', 'salon-à-supprimer')
```

## 🎲 Génération Aléatoire

```my
Discord.my.command('dice', 'Lancement de dés!')
Discord.my.random('number')

Discord.my.command('image', 'Image aléatoire!')
Discord.my.random('image')
```

## 🔧 Intégration avec Maya

Vous pouvez utiliser TOUTES les fonctionnalités Maya dans vos bots!

```my
# Variables
my.variable bot_name = 'MayaBot'
Discord.my.embed('info', 'Bot: ' + bot_name, 'Informations du bot')

# Boucles
my.for(i, 1, 5)
my.console('Initialisation étape ' + i)

# Conditions
my.variable user_level = 10
my.if(user_level >= 10)
my.alors {
    Discord.my.command('admin', 'Commande admin disponible!')
}

# Dessins ASCII
my.draw.heart
my.console('Bot configuré avec amour!')

# Couleurs
my.color.console('PINK')
my.console('Bot Maya stylé!')

# Mathématiques
my.math.random(1, 100)

# Et bien plus!
```

## 📝 Exemple Complet

```my
# Bot Discord Maya - Exemple Complet

# 1. Connexion
Discord.my.connect('YOUR_TOKEN_HERE', '!')

# 2. Configuration
Discord.my.status('en ligne')
Discord.my.activity('playing', 'Maya v6.0')

# 3. Intents
Discord.my.intent('message_content')

# 4. Commandes de base
Discord.my.command('ping', 'Pong! 🏓')
Discord.my.embed('info', 'Bot Maya', 'Créé avec Discord.my!', '#ff69b4')

# 5. Modération
Discord.my.command('clear', 'Messages supprimés!')
Discord.my.permission('clear', 'MANAGE_MESSAGES')
Discord.my.error('clear', 'Permission insuffisante!')

# 6. Intégration Maya
my.variable version = '1.0'
Discord.my.embed('version', 'Version Bot', 'Version: ' + version, '#00ff00')

# 7. Fonctionnalités créatives
Discord.my.random('number')
my.draw.heart
my.console('🤖 Bot Maya prêt!')
```

## 🎯 Conseils et Bonnes Pratiques

### 1. Sécurité
- Ne partagez JAMAIS votre token
- Utilisez des permissions appropriées
- Testez toujours vos commandes

### 2. Expérience Utilisateur
- Créez des messages d'erreur clairs
- Utilisez des embeds pour plus de style
- Documentez vos commandes

### 3. Performance
- Limitez le nombre de commandes (max 100)
- Utilisez des prefixes courts
- Optimisez vos réponses

## 🚀 Fonctionnalités Avancées

### Bot Persistant
Le bot reste actif jusqu'à arrêt manuel du processus. Parfait pour les serveurs 24/7!

### Intégration Complète Maya
Utilisez TOUTES les fonctionnalités Maya:
- Simulations
- Jeux intégrés
- Mathématiques avancées
- Génération de contenu
- Et bien plus!

### Customisation Totale
- Messages personnalisés
- Embeds avec couleurs
- Permissions granulaires
- Gestion d'erreurs complète

## 🎉 Conclusion

Discord.my révolutionne la création de bots Discord en rendant le processus:
- **Simple**: Syntaxe Maya intuitive
- **Puissant**: Toutes les fonctionnalités Discord
- **Créatif**: Intégration Maya complète
- **Accessible**: Pas besoin de connaître JavaScript ou Python

**Créez votre bot Discord en quelques minutes avec Maya v6.0!** 🚀

---

*Créé avec ❤️ par nekoclem - Maya v6.0*
