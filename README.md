# Foretold
**An Open-Source Prediction Registry for Judgemental Forecasting**

Foretold places an emphasis on distributional predictions, tables of predictions, and API support.

## The Main Concept

[Foretold](https://www.foretold.io/login) first is general-purpose prediction registry software.

The main features of a generic prediction registry include features like:

- Users can define questions to be predicted
- Forecasters can predict those questions
- Questions can either be resolved with answers or cancelled
- After questions are resolved, forecasters are scored on meaningful metrics

In addition to the essentials, Foretold includes some other features including:

**Full distribution forecasts for continuous variables**  
Continuous variables are estimated with arbitrary probability distributions. Most existing forecasting tools only allow for binary and categorical binary questions, or relatively simple distributions. [Foretold.io](https://www.foretold.io/login) saves arbitrary cumulative density functions. The [main input editor](https://observablehq.com/@oagr/foretold-inputs) is a fork of that in [Guesstimate](https://www.getguesstimate.com/). We plan to add more input methods in the future. 

**Communities with custom privacy settings**    
[Foretold.io](https://www.foretold.io/login) allows for groups to collaborate on forecasting different sets of questions. Communities can be public or private, and question creators can easily move their questions between communities. 

**A GraphQL API, with support for bots**    
Users can create bots that get scored individually. They can use the same GraphQL API that the [Foretold.io](https://www.foretold.io/login) client uses. You can see information about how to use the API [here](https://observablehq.com/@jjj/bot-tips). This part is still early, but will continue to improve.

**Notebooks with Tabular Forecasts**  
You can make "notebooks" with combinations of Markdown text and tables of forecasts. This is useful when you have organized tabular data that you would like forecasted.

In the future we hope that the API will be used to do things like:

- Make forecasts
- Make & resolve questions
- Automate the setup of new prediction experiments
- Make dashboards of useful forecasts

## Intended Uses  

Similar to Guesstimate, Foretold is not domain-specific. It could be used in multiple kinds of setups; for instance, for personal use, group use, or for a sizable open prediction tournament. Hopefully over the coming years we’ll identify which specific uses and setups are the most promising and optimize accordingly.

Recently it’s been used for:  

- Various personal/individual questions
- Internal group predictions at the Future of Humanity Institute
- A currently-open tournament on predicting the upcoming EA survey responses
- A few small forecasting experiments

We encourage broad experimentation. Feel free to make as many public & private communities as you like for different purposes. If you'd be interested in discussing possible details, please reach out.

## Questions

**I’m interested in performing an experiment that could use the tracking of probability distributions. Can I use Foretold?**  
Yes! Foretold is open-source, and we’re very happy to give special support to researchers and similar interested in working with probability distributions and/or forecasts. It’s made to be reasonably general-purpose and extendable via the API.

To get started, simply create a community on [Foretold.io](https://www.foretold.io/login) and make a few questions. If you prefer, you can also fork the codebase and run the app separately.

**Is there any connection between Foretold.io and Guesstimate?**  
[Foretold.io](https://www.foretold.io/login) uses a fork of the distribution editor from Guesstimate. The distribution syntax is the same (“5 to 20”). In the future we plan to make it easy to import [Foretold.io](https://www.foretold.io/login) variables into Guesstimate, and to use Guesstimate variables for predictions in [Foretold.io](https://www.foretold.io/login).

## Details

**Technical details**  
[Foretold.io](https://www.foretold.io/login) uses Node.js and Express.js with Apollo for the GraphQL server, and ReasonML and React for the client. The database is PostgreSQL. The application is currently hosted on Heroku.

**Funding**  
The project has raised $90,000 from the [Long-Term Future Fund](https://app.effectivealtruism.org/funds/far-future). Around $25,000 of that has been spent so far, mostly on programming and design help. 

**Ownership**  
Foretold is open source. We are currently setting a nonprofit to help support it long-term.

**Get Involved**  
Foretold is free & open to use of all (legal) kinds. That said, if you intend to make serious use of the API, please let me know beforehand.

If you’re interested in collaborating on either the platform, formal experiments, or related research, please reach out, either via private message or email. We're particularly looking for engineers and people who want to set up forecasting tournaments on important topics.

## Select Screenshots  

**Community Page**  
![community-view](https://raw.githubusercontent.com/foretold-app/foretold/master/docs/images/screenshot-community-page.png)

**Communities List**  
![communities-list](https://raw.githubusercontent.com/foretold-app/foretold/master/docs/images/screenshot-communities.png)

**Question Page**  
![question-page](https://raw.githubusercontent.com/foretold-app/foretold/master/docs/images/screenshot-question-page.png)

**Notebook Table**  
![question-page](https://raw.githubusercontent.com/foretold-app/foretold/master/docs/images/screenshot-table.png)

**Notebook Table With Sidebar Open**  
![question-page](https://raw.githubusercontent.com/foretold-app/foretold/master/docs/images/screenshot-table-with-sidebar.png)

 
