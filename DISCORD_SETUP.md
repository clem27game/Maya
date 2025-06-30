
<line_number>1</line_number>
# 🤖 Guide Complet Discord.my - Créer des Bots Discord Réels

## 📋 Prérequis

### 1. Créer un Bot Discord
1. Allez sur [Discord Developer Portal](https://discord.com/developers/applications)
2. Cliquez sur "New Application"
3. Donnez un nom à votre application
4. Allez dans l'onglet "Bot" 
5. Cliquez sur "Add Bot"
6. Copiez le Token (gardez-le secret!)

### 2. Configurer les Permissions
Dans l'onglet "Bot":
- ✅ Public Bot (si vous voulez que d'autres l'ajoutent)
- ✅ Requires OAuth2 Code Grant (optionnel)
- ✅ Message Content Intent (IMPORTANT!)
- ✅ Server Members Intent (pour certaines fonctions)
- ✅ Presence Intent (pour le statut)

### 3. Inviter le Bot sur votre Serveur
1. Onglet "OAuth2" > "URL Generator"
2. Scopes: ✅ bot ✅ applications.commands
3. Permissions Bot: 
   - ✅ Send Messages
   - ✅ Read Message History
   - ✅ Use Slash Commands
   - ✅ Manage Messages (pour modération)
   - ✅ Kick Members (optionnel)
   - ✅ Ban Members (optionnel)
   - ✅ Manage Channels (optionnel)
4. Copiez l'URL générée et ouvrez-la
5. Sélectionnez votre serveur et autorisez

## 🚀 Configuration Maya

### 1. Créer votre fichier bot (.my)
```my
# mon_bot.my
Discord.my.connect('VOTRE_TOKEN_ICI', '!')
Discord.my.status('en ligne')
Discord.my.activity('playing', 'avec Maya!')
Discord.my.intent('message_content')

Discord.my.command('ping', 'Pong!')
Discord.my.embed('info', 'Mon Bot', 'Créé avec Maya!', '#ff69b4')
```

### 2. Lancer le Bot
```bash
./main Discord.my-allume mon_bot.my
```

## 🔧 Fonctionnalités Disponibles

### Connexion et Configuration
- `Discord.my.connect(token, prefix)` - Connecter le bot
- `Discord.my.status(statut)` - en ligne, inactif, ne pas déranger
- `Discord.my.activity(type, nom)` - playing, listening, watching
- `Discord.my.intent(nom)` - Activer les intents Discord

### Commandes
- `Discord.my.command(trigger, réponse)` - Commande simple
- `Discord.my.embed(trigger, titre, description, couleur, footer, image)` - Embed

### Permissions et Sécurité
- `Discord.my.permission(commande, permission)` - Définir permissions
- `Discord.my.error(commande, message)` - Message d'erreur

### Modération
- `Discord.my.kick(user_id, raison)` - Expulser
- `Discord.my.ban(user_id, raison)` - Bannir
- `Discord.my.channel(action, nom)` - Gérer channels

### Génération Aléatoire
- `Discord.my.random(type)` - number, image

## 📊 Exemple Complet

```my
# Bot Discord Complet
Discord.my.connect('VOTRE TOKEN', '!')

# Configuration
Discord.my.status('en ligne')
Discord.my.activity('playing', 'Maya v6.0!')
Discord.my.intent('message_content')
Discord.my.intent('guild_messages')

# Commandes de base
Discord.my.command('ping', 'Pong! 🏓')
Discord.my.command('aide', 'Commandes: !ping, !aide, !info')

# Embed informatif
Discord.my.embed('info', '🤖 Mon Bot Maya', 'Bot créé avec Discord.my!', '#00ff00', 'Propulsé par Maya v6.0')

# Modération
Discord.my.command('clear', 'Messages supprimés!')
Discord.my.permission('clear', 'MANAGE_MESSAGES')
Discord.my.error('clear', 'Permission insuffisante!')

# Fonctions aléatoires
Discord.my.command('dice', 'Lancement de dés!')
Discord.my.random('number')

# Intégration Maya
my.variable bot_name = 'MayaBot'
my.console('Bot ' + bot_name + ' configuré!')
```

## 🐛 Résolution de Problèmes

### Erreur 401 - Unauthorized
- ❌ Token invalide
- ✅ Vérifiez votre token dans Discord Developer Portal

### Erreur 403 - Forbidden  
- ❌ Permissions insuffisantes
- ✅ Vérifiez les permissions du bot sur le serveur

### Erreur 50001 - Missing Access
- ❌ Bot pas invité sur le serveur
- ✅ Réinvitez le bot avec les bonnes permissions

### Bot ne répond pas
- ❌ Intent "Message Content" désactivé
- ✅ Activez dans Discord Developer Portal > Bot > Privileged Gateway Intents

### Commandes ignorées
- ❌ Mauvais prefix
- ✅ Vérifiez que vos messages commencent par le bon prefix

## 📈 Fonctionnalités Avancées

### Intégration avec Maya
```my
# Utiliser toutes les fonctions Maya dans le bot
my.create.robot('Assistant', 'aide:Je peux t\'aider!')
my.inventaire('ajouter', 'commandes', 'bot', 'Système de commandes')
my.simulation.bac() # Simulations dans Discord!
```

### Gestion d'État
```my
my.variable users_count = '0'
my.variable commands_used = '0'
Discord.my.embed('stats', 'Statistiques', 'Utilisateurs: ' + users_count, '#ffd700')
```

## 🎯 Conseils de Sécurité

1. **Ne jamais partager votre token**
2. **Utilisez des permissions minimales**
3. **Testez d'abord sur un serveur privé**
4. **Surveillez les logs pour détecter les abus**
5. **Mettez à jour régulièrement**

## 🌟 Exemples de Bots

### Bot de Modération
```my
Discord.my.connect(token, '!')
Discord.my.command('warn', 'Utilisateur averti!')
Discord.my.permission('warn', 'MODERATE_MEMBERS')
Discord.my.command('timeout', 'Utilisateur mis en timeout!')
Discord.my.permission('timeout', 'MODERATE_MEMBERS')
```

### Bot de Divertissement
```my
Discord.my.connect(token, '>')
Discord.my.random('joke')
Discord.my.command('meme', 'Voici un meme!')
Discord.my.embed('game', '🎮 Mini-Jeu', 'Tapez >play pour jouer!', '#ff6b6b')
```

### Bot Utilitaire
```my
Discord.my.connect(token, '.')
Discord.my.embed('weather', '🌤️ Météo', 'Fonctionnalité météo ici', '#87ceeb')
Discord.my.command('time', 'Il est actuellement...')
Discord.my.embed('remind', '⏰ Rappel', 'Système de rappels', '#ffa500')
```

---

**Maya Discord.my - Créez des bots Discord révolutionnaires en quelques lignes!** 🚀
