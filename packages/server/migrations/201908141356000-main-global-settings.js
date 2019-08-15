const data = [
  {
    "config": {
      "baseId": "base",
      "resourceId": "properties",
      "aliases": {}
    },
    "p-instance-of": {
      "p-name": "Instance of"
    },
    "p-name": {
      "p-name": "Name"
    },
    "p-description": {
      "p-name": "Description"
    },
    "p-url": {
      "p-name": "Web URL"
    },
    "n-organization": {
      "p-name": "Organization"
    },
    "n-movie": {
      "p-name": "Movie"
    },
    "p-author": {
      "p-name": "Author"
    }
  },
  {
    "config": {
      "baseId": "foretold",
      "resourceId": "main",
      "aliases": {
        "name": "@base/properties/p-name",
        "description": "@base/properties/p-description",
        "url": "@base/properties/p-url"
      }
    },
    "commits": {
      "name": "# of Commits"
    },
    "agents": {
      "name": "# of Agents"
    },
    "measurables": {
      "name": "# of Measurables"
    },
    "n-fhi": {
      "name": "FHI"
    },
    "n-foretold": {
      "name": "Foretold",
      "description": "This website, to hold predictions",
      "url": "foretold.io"
    },
    "n-ozzie-gooen": {
      "name": "Ozzie Gooen"
    },
    "n-jacob-lagerros": {
      "name": "Jacob Lagerros"
    }
  },
  {
    "config": {
      "baseId": "foretold",
      "resourceId": "development",
      "aliases": {
        "name": "@base/properties/p-name",
        "description": "@base/properties/p-description",
        "url": "@base/properties/p-url"
      }
    },
    "p-completion-date": {
      "name": "Date of Completion",
      "description": "When will this be deployed and available to users?"
    },
    "p-total-commits": {
      "name": "Total Commits",
      "description": "How many commits is this feature?"
    },
    "n-issue-323": {
      "name": "Email Epic",
      "description": "For this epic, we want to set up email infrastructure so that users can be emailed when important things happen. We'll test this with just one single transactional email.",
      "url": "https://github.com/OAGr/foretold/issues/329"
    },
    "n-issue-328": {
      "name": "Measurement Scoring Epic",
      "description": "The goal of this epic is to have simple 'scores' that can be used to show how good & useful agents are at predicting things. These will act as gamification to encourage users to do better.",
      "url": "https://github.com/OAGr/foretold/issues/328"
    },
    "n-functionality-brier-scrore": {
      "name": "Simple-Brier-Score",
      "description": "Implement a simple brier score system, so that users can see their brier scores on binary questions."
    }
  },
  {
    "config": {
      "baseId": "media",
      "resourceId": "movies",
      "aliases": {
        "name": "@base/properties/p-name",
        "description": "@base/properties/p-description",
        "instance-of": "@base/properties/p-instance-of",
        "url": "@base/properties/p-url",
        "movie": "@base/properties/n-movie"
      }
    },
    "imdb-rating": {
      "name": "IMDB Rating"
    },
    "rt-rating": {
      "name": "Rotten Tomatoes Rating"
    },
    "box-office-wikipedia": {
      "name": "Box Office Returns",
      "description": "Information gathered from wikipedia"
    },
    "metacritic": {
      "name": "Metacritic Rating",
      "description": "Information gathered from wikipedia"
    },
    "captain-marvel": {
      "name": "Captain Marvel",
      "url": "https://www.rottentomatoes.com/m/captain_marvel",
      "instance-of": "movie"
    },
    "dumbo": {
      "name": "Dumbo",
      "url": "https://www.rottentomatoes.com/m/dumbo",
      "instance-of": "movie"
    },
    "us": {
      "name": "Us",
      "url": "https://www.rottentomatoes.com/m/us",
      "instance-of": "movie"
    },
    "pet-semetary": {
      "name": "Pet Semetary",
      "url": "https://www.rottentomatoes.com/m/us",
      "instance-of": "movie"
    },
    "missing-link": {
      "name": "Missing Link",
      "url": "https://www.rottentomatoes.com/m/us",
      "instance-of": "movie"
    },
    "hellboy": {
      "name": "Hellboy",
      "url": "https://www.rottentomatoes.com/m/us",
      "instance-of": "movie"
    },
    "avengers-endgame": {
      "name": "Avengers: Endgame",
      "url": "https://www.rottentomatoes.com/m/avengers_endgame",
      "instance-of": "movie"
    },
    "john-wick-three": {
      "name": "John Wick: Chapter Three",
      "url": "https://www.rottentomatoes.com/m/avengers_endgame",
      "instance-of": "movie"
    },
    "detective-pikachu": {
      "name": "Detective Pikachu",
      "url": "https://www.rottentomatoes.com/m/pokemon_detective-pikachu",
      "instance-of": "movie"
    },
    "aladdin": {
      "name": "Aladdin",
      "url": "https://www.rottentomatoes.com/m/aladdin",
      "instance-of": "movie"
    },
    "the-hustle": {
      "name": "The Hustle",
      "url": "https://www.rottentomatoes.com/m/aladdin",
      "instance-of": "movie"
    },
    "rocketman": {
      "name": "Rocketman",
      "url": "https://www.rottentomatoes.com/m/aladdin",
      "instance-of": "movie"
    }
  },
  {
    "config": {
      "baseId": "lesswrong",
      "resourceId": "properties",
      "aliases": {
        "name": "@base/properties/p-name"
      }
    },
    "karma": {
      "name": "Karma"
    },
    "comments": {
      "name": "Comments(Non-Author)",
      "description": "Comments, minus those from the author"
    }
  },
  {
    "config": {
      "baseId": "lesswrong",
      "resourceId": "ozziegooen",
      "aliases": {
        "name": "@base/properties/p-name",
        "description": "@base/properties/p-description",
        "url": "@base/properties/p-url"
      }
    },
    "prediction-pyramid": {
      "name": "The Prediction Pyramid",
      "description": "The Prediction Pyramid: Why Fundamental Work is Needed for Prediction Work",
      "url": "https://www.lesswrong.com/posts/zuAj86jcB5JB4dgqm/the-prediction-pyramid-why-fundamental-work-is-needed-for"
    },
    "rain": {
      "name": "RAIN Framework!",
      "description": "The RAIN Framework for Informational Effectiveness",
      "url": "https://www.lesswrong.com/posts/s4TrCbCXvvWfkT2o6/the-rain-framework-for-informational-effectiveness"
    }
  },
  {
    "config": {
      "baseId": "lesswrong",
      "resourceId": "posts",
      "aliases": {
        "name": "@base/properties/p-name",
        "author": "@base/properties/p-author",
        "description": "@base/properties/p-description",
        "url": "@base/properties/p-url",
        "n-ozzie-gooen": "@foretold/main/n-ozzie-gooen"
      }
    },
    "FGJKj6sxx9q4FAmbS": {
      "name": "Predictive Reasoning Systems",
      "description": "The Prediction Pyramid: Why Fundamental Work is Needed for Prediction Work",
      "url": "https://www.lesswrong.com/posts/FGJKj6sxx9q4FAmbS"
    },
    "kMmNdHpQPcnJgnAQF": {
      "name": "Prediction-Augmented Evaluation Systems",
      "author": "n-ozzie-gooen"
    },
    "DCkHSrD53Methoxu6": {
      "name": "What if people simply forecasted your future choices?",
      "author": "n-ozzie-gooen"
    },
    "HB6aGCPjtvpLk4K9t": {
      "name": "Can We Place Trust in Post-AGI Forecasting Evaluations?",
      "author": "n-ozzie-gooen"
    },
    "vAzWfi5kDjZb27zD2": {
      "name": "Ideas for Next Generation Prediction Technologies",
      "author": "n-ozzie-gooen"
    },
    "zuAj86jcB5JB4dgqm": {
      "name": "The Prediction Pyramid: Why Fundamental Work is Needed for Prediction Work",
      "author": "n-ozzie-gooen"
    },
    "rain": {
      "name": "RAIN Framework",
      "description": "The RAIN Framework for Informational Effectiveness",
      "author": "n-ozzie-gooen",
      "url": "https://www.lesswrong.com/posts/s4TrCbCXvvWfkT2o6/the-rain-framework-for-informational-effectiveness"
    }
  },
  {
    "config": {
      "baseId": "orgs",
      "resourceId": "companies",
      "aliases": {
        "name": "@base/properties/p-name"
      }
    },
    "tesla": {
      "name": "Tesla"
    },
    "spacex": {
      "name": "SpaceX"
    },
    "employee-count": {
      "name": "Employee Count"
    },
    "revenue": {
      "name": "Revenue"
    },
    "market-cap": {
      "name": "Market Cap"
    }
  },
  {
    "config": {
      "baseId": "internet-intellectual-infrastructure-retreat",
      "resourceId": "main",
      "aliases": {
        "name": "@base/properties/p-name"
      }
    },
    "n-2019-retreat": {
      "name": "IIIR 2019 Retreat",
      "Description": "Internet Intellectual Infrastructure Retreat, 2019"
    },
    "p-attendance": {
      "name": "Attendance"
    },
    "p-counterfactual-value": {
      "name": "Counterfactual Value",
      "Description": "The counterfactual monetary value of a thing happening."
    }
  }
];

module.exports = {
  up: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.bulkInsert('GlobalSettings', [{
        entityGraph: JSON.stringify(data),
        name: 'main',
        createdAt: new Date(),
        updatedAt: new Date(),
      }]);
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Up Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  },

  down: async function (queryInterface) {
    try {
      await queryInterface.sequelize.query(`BEGIN`);
      await queryInterface.bulkDelete('GlobalSettings', {
        name: 'main',
      });
      await queryInterface.sequelize.query(`COMMIT`);
    } catch (e) {
      console.error('Migration Down Error', e);
      await queryInterface.sequelize.query(`ROLLBACK`);
      throw e;
    }
  }
};
